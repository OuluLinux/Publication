#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>

// Placeholder application entry point; real implementation will wire WtKernel routing.
class PublicationApp : public Wt::WApplication {
public:
    explicit PublicationApp(const Wt::WEnvironment& env)
        : Wt::WApplication(env) {
        setTitle("Publication");
    }
};

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env) {
	return std::make_unique<PublicationApp>(env);
}

int main(int argc, char** argv) {
    return Wt::WRun(argc, argv, &createApplication);
}
