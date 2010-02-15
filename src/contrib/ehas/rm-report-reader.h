/* 
 * File:   rm-report-reader.h
 * Author: patxi
 *
 * Created on 22 December 2009, 16:31
 */

#ifndef _RM_REPORT_READER_H
#define	_RM_REPORT_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ns3/type-id.h"

#include "net-data-struct.h"

namespace ns3 {

using namespace std;

/**
 * @brief A class to read Rodio Mobile reports.
 *
 */
class RmReportReader
{
public:
    /**
     * Constructor:
     */
    RmReportReader();
    /**
     * Destructor
     */
    ~RmReportReader();
    
        static TypeId GetTypeId(void);

    /**
     * @brief Reads and parses Radio Mobile report.
     *
     * This is the main method of the class.
     *
     * @param file report.txt
     *
     */
    NetDataStruct::NetData ReadRmReport(ifstream &file);


    /**
     * @brief Divides the different sections of the file into string vectors.
     *
     * @param file Radio Mobile report.
     * @param begining The line that opens the section.
     * @param header The header of the section.
     * @param section The string vector where the section will be stored.
     *
     * @return 0 if every thing is ok.
     */
    uint16_t Section2vector(ifstream &file, string begining, string header, vector<string> &section);

    uint16_t GetNodeNames(vector<string> sectionInfo, vector<string> &sectionNames);

    string GetName(string line, string::size_type max_length);

    uint16_t GetNets(vector<string> netsInfo, vector< vector<string> > &netInfoMatrix);

    uint16_t GetSubnetData(vector <vector<string> > netInfoMatrix, vector<NetDataStruct::SubnetData> &netData);

    uint16_t GetMembersData(vector<string> members, NetDataStruct::SubnetData &netData);

    

    // Some general lines
    string m_errorMsg;
    string m_dashLine;
    string m_blankLine;
    string m_doubleSpace;
    string m_space;

    NetDataStruct m_netDataStruct;

};

} // namespace ns3

#endif	/* _RM_REPORT_READER_H */
