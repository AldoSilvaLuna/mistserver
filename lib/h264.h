#pragma once
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <string>

#include "bitfields.h"
#include "bitstream.h"
#include "nal.h"

namespace h264{

  std::deque<nalu::nalData> analysePackets(const char *data, size_t len);

  /// Struct containing pre-calculated metadata of an SPS nal unit. Width and height in pixels, fps
  /// in Hz
  struct SPSMeta{
    uint32_t width;
    uint32_t height;
    double fps;
    uint8_t profile;
    uint8_t level;
    bool sep_col_plane;
    uint8_t cnt_type;
    bool gaps;     ///< Gaps in frame num allowed flag
    bool mbs_only; ///< MBS only flag
    uint16_t log2_max_frame_num;
    uint16_t log2_max_order_cnt;
    uint16_t max_ref_frames; ///< Maximum number of reference frames
  };

  /// Class for analyzing generic nal units
  class NAL{
  public:
    NAL();
    NAL(std::string &InputData);
    bool ReadData(std::string &InputData, bool raw = false);
    std::string AnnexB(bool LongIntro = false);
    std::string SizePrepended();
    int Type();
    std::string getData();

  protected:
    uint32_t chroma_format_idc; ///< the value of chroma_format_idc
    std::string MyData;         ///< The h264 nal unit data
  };
  // NAL class

  /// Special instance of NAL class for analyzing SPS nal units
  class SPS : public NAL{
  public:
    SPS() : NAL(){}
    SPS(std::string &InputData, bool raw = false);
    SPSMeta getCharacteristics();
    void analyzeSPS();
  };

  /// Special instance of NAL class for analyzing PPS nal units
  class PPS : public NAL{
  public:
    PPS() : NAL(){}
    PPS(std::string &InputData) : NAL(InputData){}
    void analyzePPS();
  };

  class sequenceParameterSet{
  public:
    sequenceParameterSet(const char *_data = NULL, size_t _dataLen = 0);
    void fromDTSCInit(const std::string &dtscInit);
    SPSMeta getCharacteristics() const;

  private:
    const char *data;
    size_t dataLen;
  };

  bool isKeyframe(const char *data, uint32_t len);

}// namespace h264

