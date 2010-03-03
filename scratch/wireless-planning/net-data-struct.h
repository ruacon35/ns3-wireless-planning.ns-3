/*
 * File:   net-data-struct.h
 * Author: patxi
 *
 * Created on 18 December 2009, 18:30
 */

#ifndef _NET_DATA_STRUCT_H
#define	_NET_DATA_STRUCT_H

#include <string>
#include <vector>

#include "ns3/type-id.h"
#include "ns3/object.h"


using namespace std;

namespace ns3 {

    /**
     * @brief Comes from Radio Mobile report.txt reading.
     */
    class NetDataStruct {
    public:

        typedef struct {
          double x;
          double y;
          double z;
        } Position;
        
        typedef struct {
            vector<string> names;
            vector<Position> positions;
            // location
            // elevation
        }NodesInfo;

        typedef struct {
            vector<string> names;
            // Pwr Tx
            // Loss ...
        } SystemInfo;
        
        /**
         * @struct Subnetwork Data
         */
        typedef struct{
            string name;
            string mode;
            vector<string> nodes;
            vector<string> roles;
            vector<string> systems;
            vector<double> distances;
        }SubnetData;

        typedef vector<SubnetData> VectorSubnetData;

        /**
         * @struct Network Data
         */
        typedef struct  {
            vector<string> generalInfo;
            NodesInfo nodesInfo;
            SystemInfo systemsInfo;
            VectorSubnetData vSubnetData;
        }NetData;

               NetDataStruct();

        ~NetDataStruct();

        static TypeId GetTypeId(void);

        void DoNothing();

    }; // class

} // namespace ns3

#endif	/* _NET_DATA_STRUCT_H */


