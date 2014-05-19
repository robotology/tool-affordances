#blobSpotter.thrift

/**
* blobSpotter_IDLServer
*
* Interface. 
*/

service blobSpotter_IDLServer
{
    /**
    * Initializes a histogram.
    * This enables the input from the roi port, 
    * then waits for the user to select region 
    * on image.
    * @return i32 of histogram index
    */
    i32 newHist();

    /**
     * Gets the current index of histograms
     * @return true/false on success/failure
     */
    i32 getCurHist();

    /**
     * Gets the total number of available histograms
     * @return true/false on success/failure
     */
    i32 getTotHist();

    /**
     * Sets which histogram to use
     * @return true/false on success/failure
     */
    bool setHist(1:i32 index);

    /**
     *  Resets all the histograms
     * @return true/false on success/failure
     */
    bool reset();

    /**
     * Quit the module.
     * @return true/false on success/failure
     */
    bool quit();


}
