#toolRecognizer.thrift

/**
* toolRecognizer_IDLServer
*
* Interface. 
*/


struct BoundingBox
{
  1: double tlx;
  2: double tly;
  3: double brx;
  4: double bry;
}

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
    bool train(1: string label, 2:double tlx = 0.0, 3:double tly = 0.0, 4:double brx = 0.0 , 5:double bry = 0.0);

    /**
     * Checks whether the hand is full or empty
     * @return true/false  corresponding to full or empty hand
     */
    string recognize(1:double tlx = 0.0, 2:double tly = 0.0, 3:double brx = 0.0 , 4:double bry = 0.0);
}
