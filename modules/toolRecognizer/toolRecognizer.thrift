#toolRecognizer.thrift

/**
* toolRecognizer_IDLServer
*
* Interface. 
*/


service toolRecognizer_IDLServer
{
    /**
     * Start the module
     * @return true/false on success/failure
     */
    bool start();

    /**
     * Quit the module
     * @return true/false on success/failure
     */
    bool quit();

    /**
     * Command to train tools by their label
     * @return true/false on success/failure to train classifiers.
     */
    bool train(1: string label = "");

    /**
     * Checks whether the hand is full or empty
     * @return true/false  corresponding to full or empty hand
     */
    string recognize();
}
