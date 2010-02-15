#include "net-data-struct.h"

namespace ns3
{

 NetDataStruct::NetDataStruct () { }

 NetDataStruct::~NetDataStruct () { }

 TypeId
 NetDataStruct::GetTypeId (void)
 {
  static TypeId tid = TypeId ("ns3::NetDataStruct")
          .SetParent<Object > ()
          ;
  return tid;
 }

 void
 NetDataStruct::DoNothing () { }

} // namespace ns3
