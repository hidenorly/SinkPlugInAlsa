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
#include "PipedSink.hpp"
#include "Buffer.hpp"
#include "SinkAlsa.hpp"
#include "SinkFilter.hpp"
#include <cmath>
#include <iostream>
#include <memory>

class PipedSinkAlsa;
class PipedSinkAlsa : public SinkPlugIn
{
protected:
  std::shared_ptr<PipedSink> mPipedSink;

  void terminate(void){
    if( mPipedSink ){
      mPipedSink->stop();
      mPipedSink->clearFilters();
      mPipedSink->detachSink();
    }
    mPipedSink.reset();
  }

  virtual void writePrimitive(IAudioBuffer& buf){
    if( mPipedSink ){
      mPipedSink->run();
      mPipedSink->writePrimitive(buf);
    }
  }

public:
  PipedSinkAlsa(){
    mPipedSink = std::make_shared<PipedSink>( );
    attachSink( std::make_shared<SinkAlsa>() );
    addFilterToTail( std::make_shared<NullFilter>() );
  };
  virtual ~PipedSinkAlsa(){
    terminate();
  };

  virtual void setAudioFormatPrimitive(AudioFormat format){
    if( mPipedSink ){
      mPipedSink->setAudioFormat(format);
    } else {
      SinkPlugIn::setAudioFormatPrimitive(format);
    }
  };

  virtual AudioFormat getAudioFormat(void){
    AudioFormat result;
    if( mPipedSink ){
      result = mPipedSink->getAudioFormat();
    }
    return result;
  }
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void){
    std::vector<AudioFormat> result;
    if( mPipedSink ){
      result = mPipedSink->getSupportedAudioFormats();
    }
    return result;
  }

  virtual bool isAvailableFormat(AudioFormat format){
    bool result = false;
    if( mPipedSink ){
      result = mPipedSink->isAvailableFormat(format);
    }
    return result;
  };


  virtual std::shared_ptr<ISink> attachSink(std::shared_ptr<ISink> pSink){
    std::shared_ptr<ISink> result;
    if( mPipedSink ){
      result = mPipedSink->attachSink( pSink );
    }
    return result;
  }
  virtual std::shared_ptr<ISink> detachSink(void){
    std::shared_ptr<ISink> result;
    if( mPipedSink ){
      result = mPipedSink->detachSink();
    }
    return result;
  }

  virtual void addFilterToHead(std::shared_ptr<IFilter> pFilter){
    if( mPipedSink ){
      mPipedSink->addFilterToHead( pFilter );
    }
  }
  virtual void addFilterToTail(std::shared_ptr<IFilter> pFilter){
    if( mPipedSink ){
      mPipedSink->addFilterToTail( pFilter );
    }
  }
  virtual int stateResourceConsumption(void){
    int result = 0;
    if( mPipedSink ){
      result = mPipedSink->stateResourceConsumption();
    }
    return result;
  }

  virtual std::vector<ISink::PRESENTATION> getAvailablePresentations(void){
    std::vector<ISink::PRESENTATION> result({ISink::PRESENTATION::PRESENTATION_DEFAULT});
    if( mPipedSink ){
      result = mPipedSink->getAvailablePresentations();
    }
    return result;
  }
  virtual bool isAvailablePresentation(ISink::PRESENTATION presentation){
    bool result = false;
    if( mPipedSink ){
      result = mPipedSink->isAvailablePresentation(presentation);
    }
    return result;
  }
  virtual bool setPresentation(ISink::PRESENTATION presentation){
    bool result = false;
    if( mPipedSink ){
      result = mPipedSink->setPresentation(presentation);
    }
    return result;
  }
  virtual ISink::PRESENTATION getPresentation(void){
    ISink::PRESENTATION result = ISink::PRESENTATION::PRESENTATION_DEFAULT;
    if( mPipedSink ){
      result = mPipedSink->getPresentation();
    }
    return result;
  }

  virtual int getLatencyUSec(void){
    int result = 0;
    if( mPipedSink ){
      result = mPipedSink->getLatencyUSec();
    }
    return result;
  }
  virtual int64_t getSinkPts(void){
    int64_t result = 0;
    if( mPipedSink ){
      result = mPipedSink->getSinkPts();
    }
    return result;
  }

  virtual void dump(void){
    if( mPipedSink ){
      mPipedSink->dump();
    }
  }

  virtual std::string toString(void){ return "PipedSinkAlsa"; };

  /* @desc initialize at loading the filter plug-in shared object such as .so */
  virtual void onLoad(void){
    std::cout << "onLoad" << std::endl;
  }
  /* @desc uninitialize at unloading the filter plug-in shared object such as .so */
  virtual void onUnload(void){
    std::cout << "onUnload" << std::endl;
    terminate();
  }
  /* @desc report your filter plug-in's unique id
     @return unique plug-in id. may use uuid. */
  virtual std::string getId(void){
    return std::string("PipedSinkAlsa");
  }
  /* @desc this is expected to use by strategy
     @return new YourFilter()'s result */
  virtual std::shared_ptr<IPlugIn> newInstance(void){
    return std::make_shared<PipedSinkAlsa>();
  }
};


extern "C"
{
void* getPlugInInstance(void)
{
  PipedSinkAlsa* pInstance = new PipedSinkAlsa();
  return reinterpret_cast<void*>(dynamic_cast<IPlugIn*>(pInstance));
}
};
