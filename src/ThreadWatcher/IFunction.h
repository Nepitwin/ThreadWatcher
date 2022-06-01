#pragma once

namespace ThreadWatcher
{
    /**
     * @brief Interface class to create thread based processing.
     *
     * @author Andreas Sekulski
     */
        class IFunction
        {

        public:

            /**
             * @brief Execution method from thread.
             */
            virtual void Process() = 0;

            /**
             * @brief Stop method for a clean Stop from thread.
             */
            virtual void Stop() = 0;
        };
}
