
//  src/devices/wifi/qap-wifi-mac.cc and qsta-wifi-mac.cc
edca->SetMaxQueueSize (30);// Patxi HARDCODED
    
//  src/devices/wifi/wifi-mac.cc  
void
WifiMac::ConfigureDcf (Ptr<Dcf> dcf, uint32_t cwmin, uint32_t cwmax, enum AccessClass ac)
{
  /* see IEE802.11 section 7.3.2.29 */
  switch (ac) {
  case AC_VO:
    dcf->SetMinCw ((cwmin+1)/4-1);
    dcf->SetMaxCw ((cwmin+1)/2-1);
    dcf->SetAifsn (2);// 2 // Patxi HARDCODED
    break;
  case AC_VI:
    dcf->SetMinCw ((cwmin+1)/2-1);
    dcf->SetMaxCw (cwmin);
    dcf->SetAifsn (3);// 2
    break;
  case AC_BE:
    dcf->SetMinCw (cwmin);
    dcf->SetMaxCw (cwmax);
    dcf->SetAifsn (4);// 3
    break;
  case AC_BK:
    dcf->SetMinCw (cwmin);
    dcf->SetMaxCw (cwmax);
    dcf->SetAifsn (5);// 7
    break;
  case AC_BE_NQOS:
    dcf->SetMinCw (cwmin);
    dcf->SetMaxCw (cwmax);
    dcf->SetAifsn (2);    
    break;
  case AC_UNDEF:
    NS_FATAL_ERROR ("I don't know what to do with this");
    break;
  }
}

// src/devices/wifi/mac-low.cc
// Preamble SHORT vs LONG
916	  return m_phy->CalculateTxDuration (GetAckSize (), ackMode, WIFI_PREAMBLE_SHORT);
