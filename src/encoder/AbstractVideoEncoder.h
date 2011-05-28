/*
 * AbstractEncoder.h
 *
 *  Created on: Apr 28, 2011
 *      Author: stefan
 */

#ifndef ABSTRACTVIDEOENCODER_H_
#define ABSTRACTVIDEOENCODER_H_

#include "AbstractEncoder.h"


class AbstractVideoEncoder : public AbstractEncoder
{

    protected:

        std::string         statfile;
        int                 gopsize;
        int                 scenecut;
        std::string         firstPassOpt;
        std::string         secondPassOpt;
        int                 width;
        int                 height;

    public:
        AbstractVideoEncoder();
        virtual ~AbstractVideoEncoder();

        virtual std::string encode(std::string) =0;
        virtual std::string encode()            =0;


        void            setStatisticFile    (std::string stat);
        std::string     getStatisticFile    ();
        void            setGOPSize          (int gop);
        int             getGOPSize          ();
        void            setSceneCut         (int scene);
        int             getSceneCut         ();
        void            setSpecFirstPassOpt (std::string opt);
        std::string     getSpecFirstPassOpt ();
        void            setSpecSecPassOpt   (std::string opt);
        std::string     getSpecSecPassOpt   ();
        void            setWidth            (int w);
        int             getWidth            ();
        void            setHeight           (int h);
        int             getHeight           ();
};
#endif /* ABSTRACTVIDEOENCODER_H_ */
