#pragma once

namespace ThreadWatcher {

    /**
     * @brief Thread conditions from process which can be reached.
     *
     * INIT -> RUN -> FINISHED (Thread finished)
     *             -> KILLED (Thread forced to kill)
     * 			   -> RUNS_AFTER_KILL (Thread can not be hard killed because of an exception)
     */
    enum Status
    {
        INIT, 			///< Initialization from process but not started
        RUN, 			///< Process is running
        FINISHED, 		///< Process is stopped clear
        KILLED, 		///< Process is hard killed
        RUNS_AFTER_KILL	///< Process can not be killed and runs always.
    };
}