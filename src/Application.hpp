#ifndef TCPDAE_APPLICATION_HPP
#define TCPDAE_APPLICATION_HPP

namespace tcpdae {

class Application final {
   public:
    ///@brief construct a new Application object
    Application() = default;

    ///@brief destroy the Application object
    ~Application() = default;

    // deleted copy, move constructors and assignment operators
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    /// @brief run application
    void run();

    ///@brief stop application
    void stop();

   private:
    // TODO
};

}  // namespace tcpdae

#endif  // TCPDAE_APPLICATION_HPP
