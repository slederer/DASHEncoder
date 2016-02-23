#include "AnyOption.h"
#include "DASHOption.h"
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
void doDash(void);
IEncoder::EncoderType getEncoder(std::string);
int convertMPD(std::string input, std::string output, std::string duration, std::string baseurl, std::string minbuffer, std::string segduration);

AnyOption *opt;
DASHOption *DASHOpt;

static DASHOption::DASHOptsList generalOptions = {
    "General options:",
    {
        {"help",              'h', OPTIONAL_ARG,  "", FLAG,   HELP("Print help")                         },
        {"input",             'i', MANDATORY_ARG, "", OPTION, HELP("Input file")                         },
        {"dest-directory",    'd', MANDATORY_ARG, "", OPTION, HELP("Destination directory")              },
        {"video-encoder",     'V', MANDATORY_ARG, "", OPTION, HELP("Video encoder")                      },
        {"audio-encoder",     'A', MANDATORY_ARG, "", OPTION, HELP("Audio encoder")                      },
        {"multiplexer",       'R', OPTIONAL_ARG,  "", OPTION, HELP("Multiplexing tool")                  },
        {"add-non-segmented", 'D', OPTIONAL_ARG,  "", FLAG,   HELP("Generate also non-segmented version")},
        {"use-ffmpeg-pipe",   'G', OPTIONAL_ARG,  "", FLAG,   HELP("FFMPEG input conversion pipe")       },
        {"ffmpeg-opt",        'g', MANDATORY_ARG, "", OPTION, HELP("Additional FFMPEG options")          },
        {"input-res",         'I', MANDATORY_ARG, "", OPTION, HELP("Resolution of input video")          },
        {"const-filesize",    'K', OPTIONAL_ARG,  "", OPTION, HELP("Encode using constant filesize")     }
    }
};

static DASHOption::DASHOptsList x264Options = {
    "x264 options:",
    {
        {"bitrate",    'b', MANDATORY_ARG, "", OPTION, HELP("Video bitrates (exmaple: see config file)")   },
        {"passes",     'k', MANDATORY_ARG, "", OPTION, HELP("Encoding passes")                             },
        {"pass1",      'x', MANDATORY_ARG, "", OPTION, HELP("Additional Options: 1st pass video encoding") },
        {"pass2",      'X', MANDATORY_ARG, "", OPTION, HELP("Additional Options: 2nd pass video encoding") },
        {"statistics", 's', MANDATORY_ARG, "", OPTION, HELP("Statistic file for multi pass video encoding")},
        {"gop",        'g', MANDATORY_ARG, "", OPTION, HELP("GOP Size")                                    },
        {"scenecut",   'c', MANDATORY_ARG, "", OPTION, HELP("Scenecut sensitivity")                        },
        {"profile",    'p', MANDATORY_ARG, "", OPTION, HELP("h.264 profile")                               },
        {"preset",     'P', MANDATORY_ARG, "", OPTION, HELP("x264 preset")                                 }
    }
};

static DASHOption::DASHOptsList FFMPEGAACOptions = {
    "FFMPEG-AAC options:",
    {
        {"audio-quality", 'a', MANDATORY_ARG, "", OPTION, HELP("Audio qualities (see config file)")},
        {"audio-input",   'I', MANDATORY_ARG, "", OPTION, HELP("Audio source")                     },
        {"audio-codec",   'C', MANDATORY_ARG, "", OPTION, HELP("Audio codec")                      }
    }
};

static DASHOption::DASHOptsList MP4BoxOptions = {
    "MP4Box options:",
    {
         {"segment-name",  'N', MANDATORY_ARG, "", OPTION, HELP("DASH segment name")            },
         {"fragment-size", 'f', MANDATORY_ARG, "", OPTION, HELP("Fragment size in seconds")     },
         {"segment-size",  'S', MANDATORY_ARG, "", OPTION, HELP("DASH segment size in seconds") },
         {"folder-prefix", 'F', MANDATORY_ARG, "", OPTION, HELP("Represenation folder prefix")  },
         {"mux-combi",     'M', OPTIONAL_ARG,  "", OPTION, HELP("A/V muxing combinations")      },
         {"rap-aligned",   'r', MANDATORY_ARG, "", FLAG,   HELP("Muliplexing at GOP boundaries")}
    }
};

static DASHOption::DASHOptsList MPDOptions = {
    "MPD options:",
    {
        {"mpd-name",              'm', MANDATORY_ARG, "",              OPTION, HELP("MPD name")                     },
        {"url-root",              'u', MANDATORY_ARG, "",              OPTION, HELP("Base url")                     },
        {"set-base-url",          'J', OPTIONAL_ARG,  "",              FLAG,   HELP("Use the base url")             },
        {"transform-mpd",         't', OPTIONAL_ARG,  "",              FLAG,   HELP("Transform MPD to act standard")},
        {"duration",              'T', MANDATORY_ARG, "",              OPTION, HELP("Content duration for MPD")     },
        {"mpdActStandardPostfix", 'a', MANDATORY_ARG, "transform-mpd", OPTION, HELP("")                             },
        {"minBufferTime",         'B', MANDATORY_ARG, "transform-mpd", OPTION, HELP("Minimum Buffer in MPD")        },
        {"segDuration",           'e', MANDATORY_ARG, "transform-mpd", OPTION, HELP("Segment Duration for MPD")     }
    }
};

int main(int argc, char* argv[])
{
    std::vector<string> mandatory;
    std::vector<string>::iterator option;

    opt = new AnyOption();
    DASHOpt = new DASHOption(opt);

    /* COMMAND LINE PREFERENCES  */
    opt->setVerbose(); /* print warnings about unknown options */
    opt->autoUsagePrint(true); /* print usage for bad options */

    DASHOpt->addOptions(&generalOptions, opt);
    DASHOpt->addOptions(&x264Options, opt);
    DASHOpt->addOptions(&FFMPEGAACOptions, opt);
    DASHOpt->addOptions(&MP4BoxOptions, opt);
    DASHOpt->addOptions(&MPDOptions, opt);

    parse(argc, argv);

    /* Only Need help? */
    if (opt->getFlag("help")) {
        opt->printUsage();
        return 0;
    }

    /*
     * check if the minimum set of options/flag required for the application
     * to run are provided
     */
    DASHOpt->checkMandatory(&generalOptions, opt, &mandatory);
    DASHOpt->checkMandatory(&x264Options, opt, &mandatory);
    DASHOpt->checkMandatory(&FFMPEGAACOptions, opt, &mandatory);
    DASHOpt->checkMandatory(&MP4BoxOptions, opt, &mandatory);
    DASHOpt->checkMandatory(&MPDOptions, opt, &mandatory);

    if (!mandatory.empty()) {

        std::cout << "DASHEncoder:"
                  << "\nFollowing options are missing:\n" << endl;

        for (option = mandatory.begin(); option != mandatory.end(); ++option) {
            std::cout << " --" << *option << endl;
        }

        std::cout << "\nPlease, make sure they are provided." << endl
                  << endl;

        return 1;
    }

    /* Generate DASH */
    doDash();

    delete opt;
    delete DASHOpt;

    return 0;
}

void parse(int argc, char* argv[])
{

    /* PROCESS THE RESOURCE FILE */
    opt->processFile("./DASHEncoder.config");

    /* PROCESS THE COMMANDLINE */
    opt->processCommandArgs(argc, argv);

}

void doDash(void) {

    std::cout << "==========DASH ENCODER===============\n";
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

    free(c1);
    free(c2);
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
