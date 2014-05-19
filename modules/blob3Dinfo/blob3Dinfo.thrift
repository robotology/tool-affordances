#blob3Dinfo.thrift

/**
* 
* blob3Dinfo_IDLServer
*
* Interface. 
*/

struct info3D
{
1: bool ACK;
2: string msg;
3: list<double> content;
}

service blob3Dinfo_IDLServer
{
    /**
     * Start the module
     * @return true/false on success/failure
     */
    bool start();
    
    /**
     * Stop the module.
     * @return true/false on success/failure
     */
    bool stop();    
    
    /**
     * Compute once the 3D position
     * @return true/false on success/failure
     */
    list<i32> blobsInRange(double range);
    
    /**
     * Compute once the 3D position
     * @return true/false on success/failure
     */
    info3D get3Dinfo();
}

