#pragma once

namespace HG::Utils::Interfaces
{
    /**
     * @brief Interface, that describes any object, that
     * can be initialized.
     */
    class Initializable
    {
    public:

        /**
         * @brief Constructor.
         */
        Initializable();

        /**
         * @brief Virtual destructor, that
         * performs deinitialization if it's required.
         */
        virtual ~Initializable();

        /**
         * @brief Method for initializing object
         * of derived class. It call `onInit` method only
         * if object was not initialized.
         */
        void init();

        /**
         * @brief Method for deinitializing object
         * of derived class. It calls `onDeinit` method only
         * if object was initialized.
         */
        void deinit();

        /**
         * @brief Method for checking is object of
         * derived class is initialized.
         */
        bool isInitialized() const;

    protected:

        virtual void onInit();

        virtual void onDeinit();

    protected:

        bool m_initialized;
    };
}