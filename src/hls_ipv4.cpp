#include "hls_ipv4.h"

void D_TOP_(
  hls::stream<ap_axiu<8,1,1,1> >& Ipv4_Data,
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
  ap_uint<32> Dest_Addr
){
#pragma HLS INTERFACE axis port=Ipv4_Data
#pragma HLS INTERFACE axis port=Mac_Data

#if 1==D_RETURN_S_AXILITE_
#pragma HLS INTERFACE s_axilite bundle=Ctrl             port=return
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x10 port=Version
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x18 port=IHL
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x20 port=Type_Of_Service
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x28 port=Total_Length
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x30 port=Identification
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x38 port=Flags
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x40 port=Fragment_Offset
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x48 port=Time_To_Live
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x50 port=Protocol
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x58 port=Source_Addr
#pragma HLS INTERFACE s_axilite bundle=Ctrl offset=0x60 port=Dest_Addr
#pragma HLS STABLE variable=Version
#pragma HLS STABLE variable=IHL
#pragma HLS STABLE variable=Type_Of_Service
#pragma HLS STABLE variable=Total_Length
#pragma HLS STABLE variable=Identification
#pragma HLS STABLE variable=Flags
#pragma HLS STABLE variable=Fragment_Offset
#pragma HLS STABLE variable=Time_To_Live
#pragma HLS STABLE variable=Protocol
#pragma HLS STABLE variable=Source_Addr
#pragma HLS STABLE variable=Dest_Addr
#endif

#if 1==D_RETURN_AP_CTRL_HS_
#pragma HLS INTERFACE ap_none port=Version
#pragma HLS INTERFACE ap_none port=IHL
#pragma HLS INTERFACE ap_none port=Type_Of_Service
#pragma HLS INTERFACE ap_none port=Total_Length
#pragma HLS INTERFACE ap_none port=Identification
#pragma HLS INTERFACE ap_none port=Flags
#pragma HLS INTERFACE ap_none port=Fragment_Offset
#pragma HLS INTERFACE ap_none port=Time_To_Live
#pragma HLS INTERFACE ap_none port=Protocol
#pragma HLS INTERFACE ap_none port=Source_Addr
#pragma HLS INTERFACE ap_none port=Dest_Addr
#pragma HLS STABLE variable=Version
#pragma HLS STABLE variable=IHL
#pragma HLS STABLE variable=Type_Of_Service
#pragma HLS STABLE variable=Total_Length
#pragma HLS STABLE variable=Identification
#pragma HLS STABLE variable=Flags
#pragma HLS STABLE variable=Fragment_Offset
#pragma HLS STABLE variable=Time_To_Live
#pragma HLS STABLE variable=Protocol
#pragma HLS STABLE variable=Source_Addr
#pragma HLS STABLE variable=Dest_Addr
#endif

  ap_uint<4> Version_=Version;
  ap_uint<4> IHL_=IHL;
  ap_uint<8> Type_Of_Service_=Type_Of_Service;
  ap_uint<16> Total_Length_=Total_Length;
  ap_uint<16> Identification_=Identification;
  ap_uint<3> Flags_=Flags;
  ap_uint<13> Fragment_Offset_=Fragment_Offset;
  ap_uint<8> Time_To_Live_=Time_To_Live;
  ap_uint<8> Protocol_=Protocol;
  ap_uint<32> Source_Addr_=Source_Addr;
  ap_uint<32> Dest_Addr_=Dest_Addr;

#pragma HLS DATAFLOW

  std::array<ap_uint<8>, Power2<Bit_Width<D_MAX_TOTAL_LENGTH_-1>::Value>::Value> Buf1;
#pragma HLS BIND_STORAGE variable=Buf1 type=RAM_T2P impl=BRAM

  std::array<ap_uint<8>, Power2<Bit_Width<D_MAX_TOTAL_LENGTH_-1>::Value>::Value> Buf2;
#pragma HLS BIND_STORAGE variable=Buf2 type=RAM_T2P impl=BRAM

  ap_uint<16> Buf1_Len;
  ap_uint<16> Buf2_Len;

  Fill_Bufs<D_MAX_TOTAL_LENGTH_>(
    Ipv4_Data,
    Buf1,Buf1_Len,
    Buf2,Buf2_Len,
    Total_Length_
  );
  Packetize_Bufs<D_MAX_TOTAL_LENGTH_>(
    Mac_Data,
    Buf1,Buf1_Len,
    Buf2,Buf2_Len,
    Version_,
    IHL_,
    Type_Of_Service_,
    Identification_,
    Flags_,
    Fragment_Offset_,
    Time_To_Live_,
    Protocol_,
    Source_Addr_,
    Dest_Addr_
  );
}
