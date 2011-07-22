/*
 * AbstractMultiplexer.h
 *
 *  Created on: Apr 29, 2011
 *      Author: stefan
 */

#ifndef ABSTRACTMULTIPLEXER_H_
#define ABSTRACTMULTIPLEXER_H_

#include "IMultiplexer.h"
#include "../DASHHelper.h"

class AbstractMultiplexer : public IMultiplexer
{
    protected:
        std::string     input;
        int             fragSize;
        int             segSize;
        bool            rapAligned;
        std::string     segName;
        std::string     outputDir;
        std::string     audioFile;

    public:
        AbstractMultiplexer();
        virtual ~AbstractMultiplexer();

        virtual     std::string multiplex       (std::string input) = 0;
        virtual     std::string multiplex       () = 0;
        virtual     std::string unSegment       (std::string input) = 0;

        void            setInput                (std::string in);
        std::string     getInput                ();
        void            setFragSize             (int frag);
        int             getFragSize             ();
        void            setSegSize              (int seg);
        int             getSegSize              ();
        void            setRAPAligned           (bool rap);
        bool            isRAPAligned            ();
        void            setSegName              (std::string name);
        std::string     getSegName              ();
        void            setOutputDir            (std::string out);
        std::string     getOutputDir            ();
        void            setAudioFile            (std::string file);
        std::string     getAudioFile            ();


};
#endif /* ABSTRACTMULTIPLEXER_H_ */
