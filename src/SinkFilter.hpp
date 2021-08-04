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

#ifndef __SINK_FILTER_HPP__
#define __SINK_FILTER_HPP__

#include "Buffer.hpp"
#include "Filter.hpp"
#include "AudioFormat.hpp"
#include <vector>
#include <string>

class NullFilter : public Filter
{
protected:
  std::vector<AudioFormat> mSupportedFormats;
public:
  NullFilter(){
    for(int anEncoding = AudioFormat::ENCODING::PCM_8BIT; anEncoding < AudioFormat::ENCODING::COMPRESSED_UNKNOWN; anEncoding++){
      for( int aChannel = AudioFormat::CHANNEL::CHANNEL_MONO; aChannel < AudioFormat::CHANNEL::CHANNEL_UNKNOWN; aChannel++){
        mSupportedFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 48000, (AudioFormat::CHANNEL)aChannel) );
        mSupportedFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 96000, (AudioFormat::CHANNEL)aChannel) );
      }
    }
  };
  virtual ~NullFilter(){};
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void){ return mSupportedFormats; };
  virtual void process(AudioBuffer& inBuf, AudioBuffer& outBuf){ outBuf = inBuf; };
  virtual std::string toString(void){ return "NullFilter"; };
};


#endif /* __SINK_FILTER_HPP__ */
