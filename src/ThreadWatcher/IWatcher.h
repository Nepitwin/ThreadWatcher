#pragma once

namespace ThreadWatcher
{
        /**
         * @brief Interface class to create thread based processing.
         */
        class IWatcher
        {

        public:
            virtual ~IWatcher() = default;

            /**
             * @brief Signalize to stop all process types.
             */
            virtual void Stop() = 0;
        };
}
