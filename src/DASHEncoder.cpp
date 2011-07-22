#include "AnyOption.h"
#include "multiplexer/MP4BoxMultiplexer.h"
#include "encoder/IEncoder.h"
#include "encoder/AbstractAudioEncoder.h"
#include "encoder/AbstractVideoEncoder.h"
#include "encoder/EncoderFactory.h"
#include "mpdgenerator/MPDGenerator.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "mysql/mysql.h"

void parse(int argc, char* argv[]);
void setHelp(AnyOption*);
void setOptions(AnyOption*);
IEncoder::EncoderType getEncoder(std::string);

AnyOption *opt;

int main(int argc, char* argv[])
{
    parse(argc, argv);
    return 0;
}

void parse(int argc, char* argv[])
{
    std::cout << "==========DASH ENCODER===============\n";
    AnyOption *opt = new AnyOption();

    /* COMMAND LINE PREFERENCES  */
    opt->setVerbose(); /* print warnings about unknown options */
    opt->autoUsagePrint(true); /* print usage for bad options */

    /* SET THE USAGE/HELP   */
    setHelp(opt);

    /* SET THE OPTION STRINGS/CHARACTERS */

    setOptions(opt);

    /* PROCESS THE RESOURCE FILE */
    opt->processFile("./DASHEncoder.config");
    /* PROCESS THE COMMANDLINE */
    opt->processCommandArgs(argc, argv);

    /* Run DASH Encoding */

    EncoderFactory* encoder_factory= new EncoderFactory();
    std::cout << "current encoder " << opt->getValue("video-encoder") << "\n";
    if(getEncoder(opt->getValue("video-encoder")) == IEncoder::x264)
        std::cout << "YES\n";

    AbstractVideoEncoder* e = (AbstractVideoEncoder*)encoder_factory->getEncoder(opt, getEncoder(opt->getValue("video-encoder")));
    AbstractAudioEncoder* a = (AbstractAudioEncoder*)encoder_factory->getEncoder(opt, getEncoder(opt->getValue("audio-encoder")));

    MPDGenerator* mpdgen = new MPDGenerator();
    MPDGenerator* mpdgenNonSeg = new MPDGenerator();
    MP4BoxMultiplexer* m = new MP4BoxMultiplexer();

    std::map<int, std::string> audio_files;
    std::map<int, int> av_mux_mapping;

    MYSQL_RES *result;

    MYSQL_ROW row;

    MYSQL *connection, mysql;

    int state;

    m->setFragSize(atoi(opt->getValue("fragment-size")));
    m->setRAPAligned(opt->getFlag("rap-aligned"));
    m->setOutputDir(opt->getValue("dest-directory"));
    m->setSegSize(atoi(opt->getValue("segment-size")));

    std::string h264file;
    vector<std::string> mpds;
    std::string folder;
    std::string foldername;
    std::string h264new;
    ifstream infile;
    std::string finalMPDhead = "";
    std::string finalMPDbody = "";
    std::string finalMPDfoot = "";
    std::string video_length;

    std::string baseURL = "";

    std::string exp_name = opt->getValue("dest-directory");
    exp_name.append(opt->getValue("mpd-name"));

    std::string exp_nameNonSeg = opt->getValue("dest-directory");
    exp_nameNonSeg.append("NonSeg");
    exp_nameNonSeg.append(opt->getValue("mpd-name"));

    ofstream mpdexportfile;
    mpdexportfile.open(exp_name.c_str());

    ofstream mpdexportfileNonSeg;
    mpdexportfileNonSeg.open(exp_nameNonSeg.c_str());


    std::string act = "";
    std::string mpd_temp;
    std::string act_line;
    std::string act_rep;

    if(opt->getFlag("set-base-url")){
       baseURL = "<BaseURL>";
       baseURL.append(opt->getValue("url-root"));
       baseURL.append("</BaseURL>");
    }

    /************* AUDIO/VIDEO MULTIPLEXING INFORMATION ******************************/

    if(opt->getValue("mux-combi")!=NULL){
        std::string muxconfig = opt->getValue("mux-combi");
        std::string currentaudio;
        std::string bitrates;
        do{
            bitrates = muxconfig.substr(0,muxconfig.find('@'));
            if(muxconfig.find('|') == std::string::npos){
                currentaudio = muxconfig.substr(muxconfig.find('@')+1);
                muxconfig = "";
            }
            else{
                currentaudio = muxconfig.substr(muxconfig.find('@')+1, muxconfig.find('|') );
                muxconfig = muxconfig.substr(muxconfig.find('|')+1);
            }
            do{
                if(bitrates.find(',') == std::string::npos){
                    av_mux_mapping[atoi(bitrates.c_str())] = atoi(currentaudio.c_str());
                    bitrates = "";
                }
                else{
                    av_mux_mapping[atoi(bitrates.substr(0,',').c_str())] = atoi(currentaudio.c_str());
                    bitrates = bitrates.substr(bitrates.find(',')+1);
                }
            }while(bitrates.size()>0);
        }while(muxconfig.size()>0);
    }

    /************************ AUDIO PROCESSING ******************************/
    std::string audio_quality = opt->getValue("audio-quality");
    char*c2 = (char*) malloc(audio_quality.length() * sizeof(char));
    strcpy(c2, audio_quality.c_str());
    char* act_audio = strtok(c2, "|");

    std::string act_quality = "";
    std::string act_audio_encoding;
    std::string act_audio_seg;

    while (act_audio != NULL)
    {

        act_quality = act_audio;
        a->setChannels(atoi(act_quality.substr(0, act_quality.find(',')).c_str()));

        act_quality = act_quality.substr(act_quality.find(',') + 1);
        a->setSamplingRate(atoi(act_quality.substr(0, act_quality.find(',')).c_str()));

        a->setBitrate(atoi(act_quality.substr(act_quality.find(',') + 1).c_str()));

        act_audio_encoding = a->encode();

        audio_files[a->getBitrate()] = act_audio_encoding;

        if (opt->getValue("mux-combi") == NULL)
        {
            /************ FOLDER CREATION **********************/

            foldername = opt->getValue("folder-prefix");
            foldername.append("_");
            foldername.append(DASHHelper::itos(a->getChannels()));
            foldername.append("ch_audio_");
            foldername.append(DASHHelper::itos(a->getBitrate()));
            foldername.append("kbit");

            folder = "mkdir ";
            folder.append(opt->getValue("dest-directory"));
            folder.append(foldername);
            system(folder.c_str());

            folder = "mv ";
            folder.append(opt->getValue("dest-directory"));
            folder.append(act_audio_encoding);
            folder.append(" ");
            folder.append(opt->getValue("dest-directory"));
            folder.append(foldername);
            system(folder.c_str());

            /************ MULTIPLEXING & SEGMENTATION **************/

            act_audio_seg = opt->getValue("dest-directory");
            act_audio_seg.append(foldername);
            act_audio_seg.append("/");
            act_audio_seg.append(opt->getValue("segment-name"));

            m->setSegName(act_audio_seg);

            act_audio_seg = opt->getValue("dest-directory");
            act_audio_seg.append(foldername);
            act_audio_seg.append("/");
            act_audio_seg.append(act_audio_encoding);

            std::string mpd = m->multiplex(act_audio_seg);

            mpds.push_back(mpd);

            /************ MPD READ **********************/

            mpd_temp = opt->getValue("dest-directory");
            mpd_temp.append(foldername);
            mpd_temp.append("/");
            mpd_temp.append(h264file.substr(0, h264file.find_last_of('.')));
            mpd_temp.append("_dash.mpd");

            infile.open(mpd_temp.c_str(), ifstream::in);

            act_rep = "";

            if (infile.is_open())
            {
                while (infile.good())
                {
                    getline(infile, act_line);
                    act_rep.append(act_line);
                    act_rep.append("\n");
                }
                infile.close();
            }
            else
                cout << "Error: Unable to open MPD file!";

            act_rep = act_rep.substr(act_rep.find("<Representation"), act_rep.find("</Representation>") - act_rep.find("<Representation") + 17);
            while (act_rep.find(m->getOutputDir()) != std::string::npos)
            {
                act_rep.replace(act_rep.find(m->getOutputDir()), m->getOutputDir().size(), opt->getValue("url-root"));
            }
            std::cout << "\nDone...\n";

            mpdgen->appendMPDbody(act_rep);
            mpdgen->appendMPDbody("\n");
        }

        act_audio = strtok(NULL, "|");
    }

    /************************ VIDEO PROCESSING ******************************/

    string bitrates = opt->getValue('b');
    char*c1 = (char*) malloc(bitrates.length() * sizeof(char));
    strcpy(c1, bitrates.c_str());
    char* pch = strtok(c1, "|");



    while (pch != NULL)
    {
        /************ ENCODING *****************************/
        act = pch;

        if (act.find('@') != std::string::npos)
        {
            std::cout << act << "\n";
            e->setWidth(atoi(act.substr(act.find('@') + 1, act.find('x') - act.find('@') - 1).c_str()));
            e->setHeight(atoi(act.substr(act.find('x') + 1).c_str()));
            e->setBitrate(atoi(act.substr(0, act.find('@')).c_str()));
        }
        else
        {
            e->setBitrate(atoi(pch));
        }

        h264file = e->encode();


        /************ FOLDER CREATION **********************/

        foldername = opt->getValue("folder-prefix");
        foldername.append("_");
        foldername.append(DASHHelper::itos(e->getBitrate()));
        foldername.append("kbit");

        folder = "mkdir ";
        folder.append(opt->getValue("dest-directory"));
        folder.append(foldername);
        system(folder.c_str());

        folder = "mv ";
        folder.append(opt->getValue("dest-directory"));
        folder.append(h264file);
        folder.append(" ");
        folder.append(opt->getValue("dest-directory"));
        folder.append(foldername);
        system(folder.c_str());

        if(opt->getValue("mux-combi")!=NULL){

            folder = "cp ";
            folder.append(opt->getValue("dest-directory"));
            folder.append(audio_files[av_mux_mapping[e->getBitrate()]]);
            folder.append(" ");
            folder.append(opt->getValue("dest-directory"));
            folder.append(foldername);
            folder.append("/");
            folder.append(audio_files[av_mux_mapping[e->getBitrate()]]);
            std::cout << "copy audio: " << folder;
            system(folder.c_str());
        }

        /************ STORE STATISTICS *********************/

        if(opt->getFlag("store-psnr")){

           mysql_init(&mysql);

           //mysql_options(&mysql, MYSQL_READ_DEFAULT_FILE,  "/opt/lampp/etc/my.cnf");
           mysql_options(&mysql, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, "/etc/ssl/certs/ComodoIntermediate.pem");


           connection = mysql_real_connect(&mysql,opt->getValue("sql-host"),opt->getValue("sql-user"),opt->getValue("sql-pw"),opt->getValue("sql-database"),0,0,0);

           if (connection == NULL)
           {
               std::cout << "MySQL Error: " << mysql_error(&mysql) << "\n";
               return ;
           }
           infile.open("out.txt", ifstream::in);

           act_rep = "";
           std::string query;
           int posStart;
           int posEnd;

           if (infile.is_open())
           {
               while (infile.good())
               {
                   getline(infile, act_line);
                   if(act_line.find("x264 [debug]: frame=") != std::string::npos){
                       //std::cout << act_line << "\n";
                       query = "INSERT INTO frames (framenr, type, ypsnr,upsnr, vpsnr, representation) Values (";

                       posStart = act_line.find("frame=")+6;
                       query.append(act_line.substr(posStart,  act_line.find("QP")-posStart));
                       query.append(", \"");

                       query.append(act_line.substr(act_line.find("Slice:")+6, 1));
                       query.append("\", ");

                       posStart = act_line.find("Y:")+2;
                       query.append(act_line.substr(posStart,  act_line.find("U:")-posStart));
                       query.append(", ");

                       posStart = act_line.find("U:")+2;
                       query.append(act_line.substr(posStart,  act_line.find("V:")-posStart));
                       query.append(", ");

                       query.append(act_line.substr(act_line.find("V:")+2));
                       query.append(", \"");

                       query.append(foldername);
                       query.append("\")");
                       //std::cout << "\n" << query;
                       mysql_query(connection, query.c_str());
                   }
               }
               std::cout << "PSNR data stored in MySQL database!";
               infile.close();
           }
           else
               cout << "Error: Unable to open Log file!";
        }


        /************ MULTIPLEXING & SEGMENTATION **************/

        h264new = opt->getValue("dest-directory");
        h264new.append(foldername);
        h264new.append("/");
        h264new.append(opt->getValue("segment-name"));

        m->setSegName(h264new);

        h264new = opt->getValue("dest-directory");
        h264new.append(foldername);
        h264new.append("/");
        h264new.append(h264file);

        if(opt->getValue("mux-combi")!=NULL){

            std::string af =opt->getValue("dest-directory");
            af.append(foldername);
            af.append("/");
            af.append(audio_files[av_mux_mapping[e->getBitrate()]]);

            m->setAudioFile(af);
        }

        std::string mpd = m->multiplex(h264new);
        mpds.push_back(mpd);



        /************ MPD READ **********************/

        mpd_temp = opt->getValue("dest-directory");
        mpd_temp.append(foldername);
        mpd_temp.append("/");
        mpd_temp.append(h264file.substr(0, h264file.find_last_of('.')));
        mpd_temp.append("_dash.mpd");

        infile.open(mpd_temp.c_str(), ifstream::in);

        act_rep = "";

        if (infile.is_open())
        {
            while (infile.good())
            {
                getline(infile, act_line);
                act_rep.append(act_line);
                act_rep.append("\n");
            }
            infile.close();
        }
        else
            cout << "Error: Unable to open MPD file!";

        act_rep = act_rep.substr(act_rep.find("<Representation"), act_rep.find("</Representation>") - act_rep.find("<Representation") + 17);



        if(opt->getFlag("add-non-segmented")){
            /************ GENERATE UNSEGMENTED FILE *****/
            std::string tmp_rep = act_rep;
            tmp_rep = m->unSegment(tmp_rep);

            while (tmp_rep.find(m->getOutputDir()) != std::string::npos)
            {
                if(opt->getFlag("set-base-url"))
                    tmp_rep.replace(tmp_rep.find(m->getOutputDir()), m->getOutputDir().size(), "");
                else
                    tmp_rep.replace(tmp_rep.find(m->getOutputDir()), m->getOutputDir().size(), opt->getValue("url-root"));
            }

            if(opt->getFlag("set-base-url")){
                std::string tmp_base = baseURL;
                tmp_base.append("\n<Init");
                tmp_rep.replace(tmp_rep.find("<Init"), 5, tmp_base);
            }
            mpdgenNonSeg->appendMPDbody(tmp_rep);
            mpdgenNonSeg->appendMPDbody("\n");
        }

        while (act_rep.find(m->getOutputDir()) != std::string::npos)
        {
            if(opt->getFlag("set-base-url"))
                act_rep.replace(act_rep.find(m->getOutputDir()), m->getOutputDir().size(), "");
            else
                act_rep.replace(act_rep.find(m->getOutputDir()), m->getOutputDir().size(), opt->getValue("url-root"));
        }

        if(opt->getFlag("set-base-url")){
            std::string tmp_base = baseURL;
            tmp_base.append("\n<Init");
            act_rep.replace(act_rep.find("<Init"), 5, tmp_base);
        }

        std::cout << "\nDone...\n";

        mpdgen->appendMPDbody(act_rep);
        mpdgen->appendMPDbody("\n");

        e->setWidth(0);
        e->setHeight(0);

        pch = strtok(NULL, "|");
    }

    std::cout << "\nWriting final MPD...\n";

    mpdexportfile << mpdgen->getMPD();
    mpdexportfile.close();

    if(opt->getFlag("add-non-segmented")){
        mpdexportfileNonSeg << mpdgenNonSeg->getMPD();
        mpdexportfileNonSeg.close();
    }
    std::cout << "\nFINISHED\n";

    delete opt;
}

void setHelp(AnyOption* opt)
{
    opt->addUsage("");
    opt->addUsage("Usage: ");
    opt->addUsage("");
    opt->addUsage(" -h  --help          Print help ");
    opt->addUsage(" -i  --input         YUV input file ");
    opt->addUsage(" -x  --x264          x264 Parameter ");
    opt->addUsage(" -s  --scenecut      use Scene cut ");
    opt->addUsage(" -c              convert Image ");
    opt->addUsage("     --name image.jpg    Image Name ");
    opt->addUsage("");
}
void setOptions(AnyOption* opt)
{
    opt->setFlag("help", 'h'); /* a flag (takes no argument), supporting long and short form */
    opt->setOption("input", 'i'); /* an option (takes an argument), supporting long and short form */
    opt->setOption("pass1", 'x'); /* an option (takes an argument), supporting only long form */
    opt->setOption("pass2", 'X');
    opt->setOption("bitrate", 'b');
    opt->setOption("statistics", 's');
    opt->setOption("gop", 'g');
    opt->setOption("scenecut", 'c');
    opt->setOption("profile", 'p');
    opt->setOption("preset", 'P');
    opt->setOption("fragment-size", 'f');
    opt->setFlag("rap-aligned", 'r');
    opt->setFlag("store-psnr", 'o');
    opt->setOption("segment-name", 'N');
    opt->setOption("segment-size", 'S');
    opt->setOption("folder-prefix", 'F');
    opt->setOption("dest-directory", 'd');
    opt->setOption("mpd-name", 'm');
    opt->setOption("url-root", 'u');
    opt->setOption("audio-quality", 'a');
    opt->setOption("audio-input", 'I');
    opt->setOption("audio-codec", 'C');
    opt->setOption("mux-combi", 'M');
    opt->setOption("video-encoder", 'V');
    opt->setOption("audio-encoder", 'A');
    opt->setOption("multiplexer", 'R');
    opt->setOption("const-filesize", 'K');
    opt->setOption("passes", 'k');
    opt->setOption("sql-host", 'y');
    opt->setOption("sql-user", 'z');
    opt->setOption("sql-pw", 'Z');
    opt->setOption("sql-database", 'Y');
    opt->setFlag("add-non-segmented", 'D');
    opt->setFlag("set-base-url", 'J');

}

IEncoder::EncoderType getEncoder(std::string e){
    if(e.compare("vp8")==0)
        return IEncoder::vp8;
    else if(e.compare("ffmpegAAC")==0)
        return IEncoder::ffmpegAAC;
    else if(e.compare("x264")==0)
        return IEncoder::x264;

    return IEncoder::x264;
}
