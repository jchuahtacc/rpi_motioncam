#ifndef __RASPIENCODER_H__
#define __RASPIENCODER_H__

#include <memory>
#include "rpi_motioncam/components/RaspiComponent.h"
#include "rpi_motioncam/RaspiPort.h"


#define VIDEO_FRAME_RATE_NUM 30
#define VIDEO_FRAME_RATE_DEN 1

namespace rpi_motioncam {
      
    struct RASPIENCODER_OPTION_S {
        MMAL_FOURCC_T encoding;
        int bitrate;
        uint32_t width;
        uint32_t height;
        uint32_t framerate;
        uint32_t intraperiod;
        uint32_t quantisationParameter;
        int bInlineHeaders;
        int immutableInput;
        MMAL_VIDEO_PROFILE_T profile;
        MMAL_VIDEO_LEVEL_T level;
        MMAL_VIDEO_INTRA_REFRESH_T intra_refresh_type;
        int inlineMotionVectors;
    };

    class RaspiEncoder : public RaspiComponent {
        public:
            static RASPIENCODER_OPTION_S createDefaultEncoderOptions();
            static shared_ptr< RaspiEncoder > create(RASPIENCODER_OPTION_S options);
            static shared_ptr< RaspiEncoder > create();
            shared_ptr< RaspiPort > input;
            shared_ptr< RaspiPort > output;
        protected:
            const int MAX_BITRATE_MJPEG = 25000000;
            const int MAX_BITRATE_LEVEL4 = 25000000;
            const int MAX_BITRATE_LEVEL42 = 62500000;
            const char* component_name();
            MMAL_STATUS_T init();
            RASPIENCODER_OPTION_S options_;
    };
}

#endif /* __RASPIENCODER_H__ */
