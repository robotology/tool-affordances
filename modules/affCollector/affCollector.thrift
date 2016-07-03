#affCollector.thrift

/**
* affCollector_IDLServer
*
* Interface. 
*/

struct Bottle{}
(
    yarp.name = "yarp::os::Bottle"
    yarp.includefile="yarp/os/Bottle.h"
)

service affCollector_IDLServer
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
     * Allows to modify online the number of possible actions
     * @return true/false on success/failure
     */
    bool setnumact(1: i32 numAct);

    /**
     * Activates a category (or creates if note previously exisiting), for which affordance data (action success rate) can be updated.
     * @return the index of the active (new or not) label
     */
    i32 setlabel(1: string label);

    /**
     * Returns the active label (object/tool/cateagory/etc, user defined).
     */
    string getlabel();

    /**
     * For the active label, update its affordance success rate for a given action, based on its effect.
     * @returns the updated success rate for that action and label
     */
    double updateAff(1: i32 act, 2: double eff, 3: i32 labI = -1);

    /**
     * Returns the success rate for all actions in the repertoire for a given known label (the active one by default).
     * if label == 'all', returns a concatenated vector with all konwn affordances.
     */
    Bottle getAffs(1: string label = "active");

    /**
     * Returns the history of effects for a given action and known label (the active one by default).
     */
    Bottle getAffHist(1: string label = "active", i32 act = -1);

    /**
     *  Based on the previously learnt affordances, returns the best label for a given action/task.
     * @return true/false on success/failure 
     */
    string selectTool(1:i32 action);

    /**
     * Clears all the learnt affordances of the active label, and sets it to unknown.
     * @return true/false on success/failure 
     */
    bool reset(1:string label = "active");

    /**
     * Clears all the learnt affordances all labels, and sets them to unknown.
     * @return true/false on success/failure 
     */
    bool clearAll();


    /**
     * Removes all teh content from the memory file. Use with precaution (requires confirmation)
     * @return true/false on success/failure
     */
    string forgetAll();

    /**
     * Saves the known labels in one file 'fileLables.txt' and the affHist in another 'fileHist.txt'
     * @return true/false on success/failure
     */
    bool savetofile(1:string label = "affFile.txt");

    /**
     * Reads labels and aff histories from files 'fileLables.txt' and the affHist in another 'fileHist.txt'
     * @return true/false on success/failure
     */
    bool readfile(1:string label = "affFile.txt");

    /**
     * Activates/Deactivates more verbose execution of the module.
     * @return true/false on success/failure 
     */
    bool verbose(1:bool verb);
}
