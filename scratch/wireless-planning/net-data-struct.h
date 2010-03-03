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
          uint32_t x;
          uint32_t y;
          uint32_t z;
        } Position;
        
        struct NodesInfo {
            vector<string> names;
            vector<Position> positions;
            // location
            // elevation
        };
        typedef struct NodesInfo NodesInfo;

        struct SystemInfo {
            vector<string> names;
            // Pwr Tx
            // Loss ...
        };
        typedef struct SystemInfo SystemInfo;

        /**
         * @struct Subnetwork Data
         */
        struct SubnetData {
            string name;
            string mode;
            vector<string> nodes;
            vector<string> roles;
            vector<string> systems;
            vector<double> distances;
        };
        typedef struct SubnetData SubnetData;

        typedef vector<SubnetData> VectorSubnetData;

        /**
         * @struct Network Data
         */
        struct NetData {
            vector<string> generalInfo;
            NodesInfo nodesInfo;
            SystemInfo systemsInfo;
            VectorSubnetData vSubnetData;
        };
        typedef struct NetData NetData;

               NetDataStruct();

        ~NetDataStruct();

        static TypeId GetTypeId(void);

        void DoNothing();

    }; // class

} // namespace ns3

#endif	/* _NET_DATA_STRUCT_H */


