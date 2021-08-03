/*
  Copyright (C) 2021 hidenorly

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Sink.hpp"
#include "Buffer.hpp"
#include "AudioFormat.hpp"
#include <vector>
#include <cmath>
#include <iostream>

#ifndef __SINK_ALSA_HPP__
#define __SINK_ALSA_HPP__

class SinkAlsa : public Sink
{
protected:
  std::vector<AudioFormat> mSupportedFormats;

public:
  SinkAlsa(AudioFormat outputFormat = AudioFormat());
  virtual ~SinkAlsa();
  virtual std::string toString(void){ return "SinkAlsa";};
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void){ return mSupportedFormats; }
  virtual AudioFormat getAudioFormat(void){ return mpBuf->getAudioFormat(); };
  virtual bool isAvailableFormat(AudioFormat format){ return format.isEncodingPcm(); };

protected:
  virtual void setAudioFormatPrimitive(AudioFormat format){
    if( mpBuf ){
      mpBuf->setAudioFormat( format );
    }
  };
  virtual void writePrimitive(IAudioBuffer& buf);
};


#endif /* __SINK_ALSA_HPP__ */