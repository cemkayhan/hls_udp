#include "hls_ipv4.h"

int main()
{
  hls::stream<ap_axiu<8,1,1,1> > Ipv4_Data;
  hls::stream<ap_axiu<8,1,1,1> > Mac_Data;
  ap_uint<4> Version=4;
  ap_uint<4> IHL=5;
  ap_uint<8> Type_Of_Service=0;
  ap_uint<16> Total_Length=460;
  ap_uint<16> Identification=0xfca9;
  ap_uint<3> Flags=0;
  ap_uint<13> Fragment_Offset=0;
  ap_uint<8> Time_To_Live=2;
  ap_uint<8> Protocol=23;
  ap_uint<32> Source_Addr=0x0A141E28;
  ap_uint<32> Dest_Addr=0x01020304;

  ap_axiu<8,1,1,1> Ipv4_Data_;
  Ipv4_Data_.last=0;
  for(auto i=0;i<Total_Length-1;++i){
    Ipv4_Data_.data=i;
    Ipv4_Data<<Ipv4_Data_;
  }
  Ipv4_Data_.data=0xFF;
  Ipv4_Data_.last=1;
  Ipv4_Data<<Ipv4_Data_;

  Ipv4_Data_.last=0;
  for(auto i=0;i<Total_Length-1;++i){
    Ipv4_Data_.data=i;
    Ipv4_Data<<Ipv4_Data_;
  }
  Ipv4_Data_.data=0xFF;
  Ipv4_Data_.last=1;
  Ipv4_Data<<Ipv4_Data_;

  D_TOP_(
    Ipv4_Data,
    Mac_Data,
    Version,
    IHL,
    Type_Of_Service,
    Total_Length,
    Identification,
    Flags,
    Fragment_Offset,
    Time_To_Live,
    Protocol,
    Source_Addr,
    Dest_Addr
  );

  while(!Mac_Data.empty()){
    static auto Dummy_Cntr=0;
    std::cout << std::dec << Dummy_Cntr++ << ": " << std::hex << Mac_Data.read().data << '\n';
  }
}
