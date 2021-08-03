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
#include "AudioFormatAdaptor.hpp"
#include "SinkAlsa.hpp"
#include "AlsaPrimitive.hpp"

SinkAlsa::SinkAlsa(AudioFormat outputFormat):Sink()
{
  for(int anEncoding = AudioFormat::ENCODING::PCM_8BIT; anEncoding < AudioFormat::ENCODING::COMPRESSED_UNKNOWN; anEncoding++){
    for( int aChannel = AudioFormat::CHANNEL::CHANNEL_MONO; aChannel < AudioFormat::CHANNEL::CHANNEL_UNKNOWN; aChannel++){
      mSupportedFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 48000, (AudioFormat::CHANNEL)aChannel) );
      mSupportedFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 96000, (AudioFormat::CHANNEL)aChannel) );
    }
  }
  mpBuf->setAudioFormat( outputFormat );
  AlsaPrimitive::initialize();
}

SinkAlsa::~SinkAlsa()
{
  AlsaPrimitive::terminate();
}

void SinkAlsa::writePrimitive(IAudioBuffer& buf)
{
  AudioBuffer* pSrcBuf = dynamic_cast<AudioBuffer*>(&buf);
  if( pSrcBuf ){
    AudioFormat bufFormat = mpBuf->getAudioFormat();
    if( !pSrcBuf->getAudioFormat().equal(bufFormat) ){
      mpBuf.reset();
      mpBuf = std::make_shared<AudioBuffer>( bufFormat, 0 );
      AudioBuffer outBuf( bufFormat, pSrcBuf->getNumberOfSamples() );
      AudioFormatAdaptor::convert( *pSrcBuf, outBuf );
      AlsaPrimitive::write( outBuf );
    } else {
      AlsaPrimitive::write( buf );
    }
  } else {
    // non Pcm Audio Buffer
    AlsaPrimitive::write( buf );
  }
}
