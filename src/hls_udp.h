#if !defined(HLS_UDP_H_INCLUDE_GUARD_)
#define HLS_UDP_H_INCLUDE_GUARD_

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "bit_width.h"
#include "power2.h"
#include <array>

void D_TOP_(
  hls::stream<ap_axiu<8,1,1,1> >& Udp_Data,
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Data,
  ap_uint<4> Version,
  ap_uint<4> IHL,
  ap_uint<8> Type_Of_Service,
  ap_uint<16> Total_Length,
  ap_uint<16> Identification,
  ap_uint<3> Flags,
  ap_uint<13> Fragment_Offset,
  ap_uint<8> Time_To_Live,
  ap_uint<8> Protocol,
  ap_uint<32> Source_Addr,
  ap_uint<32> Dest_Addr,
  ap_uint<16> Source_Port,
  ap_uint<16> Dest_Port
);

template<int MAX_TOTAL_LENGTH_>
static void Fill_Bufs(
  hls::stream<ap_axiu<8,1,1,1> >& Udp_Data,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_TOTAL_LENGTH_-1>::Value>::Value>& Buf1,
  ap_uint<16>& Buf1_Len,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_TOTAL_LENGTH_-1>::Value>::Value>& Buf2,
  ap_uint<16>& Buf2_Len,
  ap_uint<16> Total_Length
){
  ap_uint<16> Buf1_Len_=0;
  ap_axiu<8,1,1,1> Buf1_Data;
  loopBuf1: do{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MAX_TOTAL_LENGTH_ max=MAX_TOTAL_LENGTH_
    Udp_Data>>Buf1_Data;
    Buf1[Buf1_Len_++]=Buf1_Data.data;
  }while(!Buf1_Data.last&&Total_Length>Buf1_Len_);
  Buf1_Len=Buf1_Len_;

  ap_uint<16> Buf2_Len_=0;
  ap_axiu<8,1,1,1> Buf2_Data;
  loopBuf2: do{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=MAX_TOTAL_LENGTH_ max=MAX_TOTAL_LENGTH_
    Udp_Data>>Buf2_Data;
    Buf2[Buf2_Len_++]=Buf2_Data.data;
  }while(!Buf2_Data.last&&Total_Length>Buf2_Len_);
  Buf2_Len=Buf2_Len_;
}

template<int MAX_TOTAL_LENGTH_>
static void Packetize_Buf(
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Data,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_TOTAL_LENGTH_-1>::Value>::Value>& Buf,
  ap_uint<16>& Buf_Len,
  ap_uint<4> Version,
  ap_uint<4> IHL,
  ap_uint<8> Type_Of_Service,
  ap_uint<16> Identification,
  ap_uint<3> Flags,
  ap_uint<13> Fragment_Offset,
  ap_uint<8> Time_To_Live,
  ap_uint<8> Protocol,
  ap_uint<32> Source_Addr,
  ap_uint<32> Dest_Addr,
  ap_uint<16> Source_Port,
  ap_uint<16> Dest_Port
){
#pragma HLS INLINE

  ap_uint<32> Header_Checksum=0;
  ap_uint<16> UDP_Length=Buf_Len+8;
  ap_uint<16> Total_Length=UDP_Length+ap_uint<6> {(IHL,ap_uint<2> {0})};

  for(auto I=0;I<1+Total_Length;++I){
#pragma HLS PIPELINE II=1
    if(0==I){
      Header_Checksum+=ap_uint<16> {(Version,IHL,Type_Of_Service)};
      Header_Checksum+=Total_Length;
      Header_Checksum+=Identification;
      Header_Checksum+=ap_uint<16> {(Flags,Fragment_Offset)};
      Header_Checksum+=ap_uint<16> {(Time_To_Live,Protocol)};
      Header_Checksum+=Source_Addr(31,16);
      Header_Checksum+=Source_Addr(15,0);
      Header_Checksum+=Dest_Addr(31,16);
      Header_Checksum+=Dest_Addr(15,0);
      Header_Checksum=ap_uint<16> {Header_Checksum(31,16)+Header_Checksum(15,0)};
    }

    else if(1==I) Mac_Data<< ap_axiu<8,1,1,1> {ap_uint<8> {(Version,IHL)},-1,-1,0,0,0,0};
    else if(2==I) Mac_Data<< ap_axiu<8,1,1,1> {Type_Of_Service,-1,-1,0,0,0,0};
    else if(3==I) Mac_Data<< ap_axiu<8,1,1,1> {Total_Length(15,8),-1,-1,0,0,0,0};
    else if(4==I) Mac_Data<< ap_axiu<8,1,1,1> {Total_Length(7,0),-1,-1,0,0,0,0};
    else if(5==I) Mac_Data<< ap_axiu<8,1,1,1> {Identification(15,8),-1,-1,0,0,0,0};
    else if(6==I) Mac_Data<< ap_axiu<8,1,1,1> {Identification(7,0),-1,-1,0,0,0,0};
    else if(7==I) Mac_Data<< ap_axiu<8,1,1,1> {ap_uint<8> {(Flags,Fragment_Offset(12,8))},-1,-1,0,0,0,0};
    else if(8==I) Mac_Data<< ap_axiu<8,1,1,1> {Fragment_Offset(7,0),-1,-1,0,0,0,0};
    else if(9==I) Mac_Data<< ap_axiu<8,1,1,1> {Time_To_Live,-1,-1,0,0,0,0};
    else if(10==I) Mac_Data<< ap_axiu<8,1,1,1> {Protocol,-1,-1,0,0,0,0};
    else if(11==I) Mac_Data<< ap_axiu<8,1,1,1> {~Header_Checksum(15,8),-1,-1,0,0,0,0};
    else if(12==I) Mac_Data<< ap_axiu<8,1,1,1> {~Header_Checksum(7,0),-1,-1,0,0,0,0};
    else if(13==I) Mac_Data<< ap_axiu<8,1,1,1> {Source_Addr(31,24),-1,-1,0,0,0,0};
    else if(14==I) Mac_Data<< ap_axiu<8,1,1,1> {Source_Addr(23,16),-1,-1,0,0,0,0};
    else if(15==I) Mac_Data<< ap_axiu<8,1,1,1> {Source_Addr(15,8),-1,-1,0,0,0,0};
    else if(16==I) Mac_Data<< ap_axiu<8,1,1,1> {Source_Addr(7,0),-1,-1,0,0,0,0};
    else if(17==I) Mac_Data<< ap_axiu<8,1,1,1> {Dest_Addr(31,24),-1,-1,0,0,0,0};
    else if(18==I) Mac_Data<< ap_axiu<8,1,1,1> {Dest_Addr(23,16),-1,-1,0,0,0,0};
    else if(19==I) Mac_Data<< ap_axiu<8,1,1,1> {Dest_Addr(15,8),-1,-1,0,0,0,0};
    else if(20==I) Mac_Data<< ap_axiu<8,1,1,1> {Dest_Addr(7,0),-1,-1,0,0,0,0};
    else if(21==I) Mac_Data<< ap_axiu<8,1,1,1> {Source_Port(15,8),-1,-1,0,0,0,0};
    else if(22==I) Mac_Data<< ap_axiu<8,1,1,1> {Source_Port(7,0),-1,-1,0,0,0,0};
    else if(23==I) Mac_Data<< ap_axiu<8,1,1,1> {Dest_Port(15,8),-1,-1,0,0,0,0};
    else if(24==I) Mac_Data<< ap_axiu<8,1,1,1> {Dest_Port(7,0),-1,-1,0,0,0,0};
    else if(25==I) Mac_Data<< ap_axiu<8,1,1,1> {UDP_Length(15,8),-1,-1,0,0,0,0};
    else if(26==I) Mac_Data<< ap_axiu<8,1,1,1> {UDP_Length(7,0),-1,-1,0,0,0,0};
    else if(27==I) Mac_Data<< ap_axiu<8,1,1,1> {ap_uint<8> {0},-1,-1,0,0,0,0};
    else if(28==I) Mac_Data<< ap_axiu<8,1,1,1> {ap_uint<8> {0},-1,-1,0,0,0,0};

    else if(Total_Length==I) Mac_Data<<ap_axiu<8,1,1,1> {Buf[I-29],-1,-1,0,1,0,0};
    else Mac_Data<<ap_axiu<8,1,1,1> {Buf[I-29],-1,-1,0,0,0,0};
  }
}

template<int MAX_TOTAL_LENGTH_>
static void Packetize_Bufs(
  hls::stream<ap_axiu<8,1,1,1> >& Mac_Data,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_TOTAL_LENGTH_-1>::Value>::Value>& Buf1,
  ap_uint<16>& Buf1_Len,
  std::array<ap_uint<8>, Power2<Bit_Width<MAX_TOTAL_LENGTH_-1>::Value>::Value>& Buf2,
  ap_uint<16>& Buf2_Len,
  ap_uint<4> Version,
  ap_uint<4> IHL,
  ap_uint<8> Type_Of_Service,
  ap_uint<16> Identification,
  ap_uint<3> Flags,
  ap_uint<13> Fragment_Offset,
  ap_uint<8> Time_To_Live,
  ap_uint<8> Protocol,
  ap_uint<32> Source_Addr,
  ap_uint<32> Dest_Addr,
  ap_uint<16> Source_Port,
  ap_uint<16> Dest_Port
){
  Packetize_Buf<MAX_TOTAL_LENGTH_>(
    Mac_Data,
    Buf1,Buf1_Len,
    Version,
    IHL,
    Type_Of_Service,
    Identification,
    Flags,
    Fragment_Offset,
    Time_To_Live,
    Protocol,
    Source_Addr,
    Dest_Addr,
    Source_Port,
    Dest_Port
  );
  Packetize_Buf<MAX_TOTAL_LENGTH_>(
    Mac_Data,
    Buf2,Buf2_Len,
    Version,
    IHL,
    Type_Of_Service,
    Identification,
    Flags,
    Fragment_Offset,
    Time_To_Live,
    Protocol,
    Source_Addr,
    Dest_Addr,
    Source_Port,
    Dest_Port
  );
}

#endif
