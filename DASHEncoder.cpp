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

void parse(int argc, char* argv[]);
void setHelp(AnyOption*);
void setOptions(AnyOption*);
IEncoder::EncoderType getEncoder(std::string);
int convertMPD(std::string input, std::string output, std::string duration, std::string baseurl, std::string minbuffer, std::string segduration);

AnyOption *opt;

int main(int argc, char* argv[])
{
    std::cout << "==========DASH ENCODER===============\n";

    parse(argc, argv);
    doDash();
    return 0;
}

void parse(int argc, char* argv[])
{
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

    /* Only Need help? */
    if (opt->getFlag("help"))
        opt->printUsage();
}

void doDash(void) {
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
    std::string video_length = "";

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

    /* MPD Gneration */
    std::cout << "\nWriting final MPD...\n";

    mpdexportfile << mpdgen->getMPD();
    mpdexportfile.close();

    if(opt->getFlag("transform-mpd")){
       std::cout << "\nConverting MPDs to actual standard ...";
       convertMPD(exp_name.c_str(), opt->getValue("mpdActStandardPostfix"),  opt->getValue("duration"), opt->getValue("url-root"), opt->getValue("minBufferTime"), opt->getValue("segDuration"));
    }

    /* Non-Segmented MPD Gneration */
    if(opt->getFlag("add-non-segmented")){
        mpdexportfileNonSeg << mpdgenNonSeg->getMPD();
        mpdexportfileNonSeg.close();

        /* Run MPD Converion */
        if(opt->getFlag("transform-mpd")){
            std::cout << "\nConverting MPDs to actual standard ...";
            convertMPD(exp_nameNonSeg.c_str(), opt->getValue("mpdActStandardPostfix"),  opt->getValue("duration"), opt->getValue("url-root"),opt->getValue("minBufferTime"), opt->getValue("segDuration"));
        }
    }

    std::cout << "\nFINISHED\n";

    delete opt;

    free(c1);
    free(c2);
}

void setHelp(AnyOption* opt)
{
    opt->addUsage("");
    opt->addUsage("Usage: ");
    opt->addUsage("");
    opt->addUsage(" -h  --help              Print help ");
    opt->addUsage(" -i  --input             Input file");
    opt->addUsage(" -x  --pass1             Additional Options: 1st pass video encoding");
    opt->addUsage(" -X  --pass2             Additional Options: 2nd pass video encoding");
    opt->addUsage(" -b  --bitrate           Video bitrates (exmaple: see config file)");
    opt->addUsage(" -s  --statistics        Statistic file for multi pass video encoding");
    opt->addUsage(" -g  --gop               GOP Size");
    opt->addUsage(" -c  --scenecut          Scenecut sensitivity");
    opt->addUsage(" -p  --profile           h.264 profile");
    opt->addUsage(" -P  --preset            x264 preset");
    opt->addUsage(" -f  --fragment-size     Fragment size in seconds");
    opt->addUsage(" -r  --rap-aligned       Muliplexing at GOP boundaries");
    opt->addUsage(" -o  --store-psnr        Store PSNR statistics to database");
    opt->addUsage(" -N  --segment-name      DASH segment name");
    opt->addUsage(" -S  --segment-size      DASH segment size in seconds");
    opt->addUsage(" -F  --folder-prefix     Represenation folder prefix");
    opt->addUsage(" -d  --dest-directory    Destination directory");
    opt->addUsage(" -m  --mpd-name          MPD name");
    opt->addUsage(" -u  --url-root          Base url");
    opt->addUsage(" -a  --audio-quality     Audio qualities (see config file)");
    opt->addUsage(" -I  --audio-input       Audio source");
    opt->addUsage(" -C  --audio-codec       Audio codec");
    opt->addUsage(" -M  --mux-combi         A/V muxing combinations");
    opt->addUsage(" -V  --video-encoder     Video encoder");
    opt->addUsage(" -A  --audio-encoder     Audio encoder");
    opt->addUsage(" -R  --multiplexer       Multiplexing tool");
    opt->addUsage(" -K  --const-filesize    Encode using constant filesize");
    opt->addUsage(" -k  --passes            Encoding passes");
    opt->addUsage(" -D  --add-non-segmented Generate also non-segmented version");
    opt->addUsage(" -J  --set-base-url      Use the base url");
    opt->addUsage(" -G  --use-ffmpeg-pipe   FFMPEG input conversion pipe");
    opt->addUsage(" -g  --ffmpeg-opt        Additional FFMPEG options");
    opt->addUsage(" -I  --input-res         Resolution of input video");
    opt->addUsage(" -t  --transform-mpd     Transform MPD to act standard");
    opt->addUsage(" -T  --duration          Content duration for MPD");
    opt->addUsage(" -a  --mpdActStandardPostfix ");
    opt->addUsage(" -B  --minBufferTime     Minimum Buffer in MPD");
    opt->addUsage(" -e  --segDuration       Segment Duration for MPD");
    opt->addUsage("");
}

void setOptions(AnyOption* opt)
{
    opt->setFlag("help", 'h');
    opt->setOption("input", 'i');
    opt->setOption("pass1", 'x');
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
    opt->setFlag("add-non-segmented", 'D');
    opt->setFlag("set-base-url", 'J');
    opt->setFlag("use-ffmpeg-pipe", 'G');
    opt->setOption("ffmpeg-opt", 'g');
    opt->setOption("input-res", 'I');
    opt->setFlag("transform-mpd", 't');
    opt->setOption("duration", 'T');
    opt->setOption("mpdActStandardPostfix", 'a');
    opt->setOption("minBufferTime", 'B');
    opt->setOption("segDuration", 'e');
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

int convertMPD(std::string input, std::string output, std::string duration, std::string baseurl, std::string minbuffer, std::string segduration){
    std::cout << "\nConvert the to act DASH standard: " << input << "\nto: " << output << "\n";

    string mpd_temp = "", act_line = "", temp;
    string filename = input;

    temp = ".mpd";
    filename.replace(filename.find(temp), temp.length(),output);

    ofstream mpdexportfile;
    mpdexportfile.open(filename.c_str());


    ifstream infile;

    infile.open(input.c_str(), ifstream::in);



    if (infile.is_open())
    {
        while (infile.good())
        {
            getline(infile, act_line);
            mpd_temp.append(act_line);
            mpd_temp.append("\n");
        }
        infile.close();
    }
    else
        cout << "Error: Unable to open MPD file!";

    string finalMPDhead, finalMPDfoot, finalMPDbody;
    int pos;

    finalMPDhead.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    finalMPDhead.append("<MPD xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
    finalMPDhead.append("     xmlns=\"urn:mpeg:DASH:schema:MPD:2011\"\n");
    finalMPDhead.append("     xsi:schemaLocation=\"urn:mpeg:DASH:schema:MPD:2011\"\n");
    finalMPDhead.append("     profiles= \"urn:mpeg:dash:profile:isoff-main:2011\"\n");
    finalMPDhead.append("     type=\"static\"\n");

    //adding Duration
    finalMPDhead.append("     mediaPresentationDuration=\"PT");
    finalMPDhead.append(duration);
    finalMPDhead.append("\"\n");

    //adding Buffer Time
    finalMPDhead.append("     minBufferTime=\"PT");
    finalMPDhead.append(minbuffer);
    finalMPDhead.append("\">   \n");
/*    pos = mpd_temp.find("minBufferTime");
    if(pos != std::string::npos){
        pos = pos + 15;
        temp = mpd_temp.substr(pos,mpd_temp.find("\"", pos)- pos );
        pos = temp.length(); //1000
        string millisec, sec;
        millisec = temp.substr(pos-3,1);
        sec = temp.substr(0,pos-3);
        finalMPDhead.append(sec);
        finalMPDhead.append(".");
        finalMPDhead.append(millisec);
        finalMPDhead.append("\">   \n");
    }
    else{
        std::cout << "ERROR: Could not find buffer time!\n";
        std::cout << mpd_temp;
        return 1;
    }*/

    //adding base URL
    finalMPDhead.append("     <BaseURL>");
    finalMPDhead.append(baseurl);
    finalMPDhead.append("</BaseURL>\n");
    finalMPDhead.append("     <Period start=\"PT0S\">\n");
    finalMPDhead.append("          <AdaptationSet bitstreamSwitching=\"true\" >\n");
    mpdexportfile << finalMPDhead;


    finalMPDfoot.append("          </AdaptationSet>\n");
    finalMPDfoot.append("    </Period>\n");
    finalMPDfoot.append("</MPD>\n");


    finalMPDbody = "";

    int lastRep = 0, id = 0;
    string act_rep, finalRep, temp2;


    while(mpd_temp.find("<Representation", lastRep)!= std::string::npos){
            act_rep = mpd_temp.substr(mpd_temp.find("<Representation", lastRep), mpd_temp.find("</Representation>", lastRep)+ 17 - mpd_temp.find("<Representation", lastRep) );
            lastRep = mpd_temp.find("</Representation>", lastRep)+ 17;

            finalRep = "";

            string addID = "<Representation id=\"";
            addID.append(DASHHelper::itos(id));
            addID.append("\"");

            temp2 = "<Representation";
            act_rep.replace(act_rep.find(temp2), temp2.length(),addID);
            temp2 = "startWithRAP=\"true\"";
            act_rep.replace(act_rep.find(temp2), temp2.length(),"startWithSAP=\"1\"");

            temp2 = "mimeType=\"video/mp4\"";
            act_rep.replace(act_rep.find(temp2), temp2.length(),"codecs=\"avc1\" mimeType=\"video/mp4\"");

            temp = act_rep.substr(0, act_rep.find(">"));
            temp.append(">\n");
            finalRep.append(temp);


            //add init segment
            pos = act_rep.find("<InitialisationSegmentURL");
            temp = act_rep.substr(pos,act_rep.find("/>", pos)+2 - pos);
            temp2 = "InitialisationSegmentURL";
            temp.replace(temp.find(temp2), temp2.length(), "Initialisation");
            pos = act_rep.find("/>", pos)+2;

            finalRep.append("<SegmentBase>\n    ");
            finalRep.append(temp);
            finalRep.append("\n</SegmentBase>\n");

            //add segments
            finalRep.append("<SegmentList duration=\"");
            finalRep.append(segduration);
            finalRep.append("\">");
            temp = act_rep.substr(pos, act_rep.find("</SegmentInfo>",pos) -pos);
            temp2 = "<Url";
            while(temp.find(temp2)!= std::string::npos){
                temp.replace(temp.find(temp2), temp2.length(),"<SegmentURL");
                string temp3 = "sourceURL";
                temp.replace(temp.find(temp3), temp3.length(),"media");
                string temp4 = "range";
                if(temp.find(temp4)!= std::string::npos)
                    temp.replace(temp.find(temp4), temp4.length(),"mediaRange");
            }
            finalRep.append(temp);
            finalRep.append("</SegmentList>\n");
            finalRep.append("</Representation>\n");

            mpdexportfile << finalRep;
            id++;
    }


    mpdexportfile << finalMPDfoot;
    mpdexportfile.close();

    return 0;
}
