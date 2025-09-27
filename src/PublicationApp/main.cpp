#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPasswordEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

#include <memory>
#include <string>
#include <vector>

#include <sqlite3.h>

#include <cstdlib>
#include <filesystem>
#include <stdexcept>

namespace {
struct ArticleBrief {
    std::string section;
    std::string headline;
    std::string dek;
    std::string timestamp;
};

class UserStore {
public:
    explicit UserStore(const std::string& path) {
        if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
            throw std::runtime_error("Failed to open user store: " + std::string(sqlite3_errmsg(db_)));
        }
        exec("CREATE TABLE IF NOT EXISTS users (\n"
             "  username TEXT PRIMARY KEY,\n"
             "  password TEXT NOT NULL,\n"
             "  developer_only INTEGER DEFAULT 0 CHECK(developer_only IN (0,1))\n"
             ");");
    }

    ~UserStore() {
        if (db_) {
            sqlite3_close(db_);
            db_ = nullptr;
        }
    }

    void ensureDevUser() {
        static const char* sql =
            "INSERT OR REPLACE INTO users(username, password, developer_only) VALUES (?1, ?2, 1);";
        sqlite3_stmt* stmt = nullptr;
        prepare(sql, &stmt);
        sqlite3_bind_text(stmt, 1, "user", -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, "123", -1, SQLITE_STATIC);
        stepAndFinalize(stmt);
    }

    bool validateCredentials(const std::string& username, const std::string& password, bool developerMode) {
        static const char* sql =
            "SELECT password, developer_only FROM users WHERE username = ?1";
        sqlite3_stmt* stmt = nullptr;
        prepare(sql, &stmt);
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        bool valid = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* storedPassword = sqlite3_column_text(stmt, 0);
            int developerOnly = sqlite3_column_int(stmt, 1);
            const std::string passwordValue = storedPassword ? reinterpret_cast<const char*>(storedPassword) : "";
            if (passwordValue == password && (developerOnly == 0 || developerMode)) {
                valid = true;
            }
        }

        sqlite3_finalize(stmt);
        return valid;
    }

private:
    sqlite3* db_ = nullptr;

    void exec(const std::string& sql) {
        char* err = nullptr;
        if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            std::string message = err ? err : "unknown error";
            sqlite3_free(err);
            throw std::runtime_error("SQLite exec error: " + message);
        }
    }

    void prepare(const char* sql, sqlite3_stmt** stmt) {
        if (sqlite3_prepare_v2(db_, sql, -1, stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("SQLite prepare error: " + std::string(sqlite3_errmsg(db_)));
        }
    }

    static void stepAndFinalize(sqlite3_stmt* stmt) {
        if (!stmt) {
            return;
        }
        int rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("SQLite step error: " + std::to_string(rc));
        }
        sqlite3_finalize(stmt);
    }
};
}

class PublicationApp : public Wt::WApplication {
public:
    explicit PublicationApp(const Wt::WEnvironment& env)
        : Wt::WApplication(env) {
        setTitle("Publication");
        developerMode_ = detectDeveloperMode();
        initializeUserStore();
        configureTheme();
        loadAssets();
        buildInterface();
    }

private:
    bool developerMode_ = false;
    std::unique_ptr<UserStore> userStore_;
    Wt::WContainerWidget* userBar_ = nullptr;
    Wt::WLineEdit* usernameField_ = nullptr;
    Wt::WLineEdit* passwordField_ = nullptr;
    Wt::WText* feedback_ = nullptr;
    std::string currentUser_;

    void configureTheme() {
        auto theme = std::make_shared<Wt::WBootstrapTheme>();
        theme->setResponsive(true);
        setTheme(theme);
        setBodyClass("publication-app");
    }

    bool detectDeveloperMode() const {
        const char* envFlag = std::getenv("PUBLICATION_DEV_MODE");
        if (!envFlag) {
            return false;
        }
        const std::string flagValue = envFlag;
        return !(flagValue == "0" || flagValue == "false" || flagValue == "FALSE");
    }

    void initializeUserStore() {
        try {
            const std::filesystem::path dataDir{"data"};
            std::error_code ec;
            std::filesystem::create_directories(dataDir, ec);
            if (ec) {
                throw std::runtime_error("Failed to create data directory: " + ec.message());
            }
            const std::filesystem::path sqlitePath = dataDir / "publication.sqlite";
            userStore_ = std::make_unique<UserStore>(sqlitePath.string());
            if (developerMode_) {
                userStore_->ensureDevUser();
                Wt::log("info") << "Developer mode: seeded dummy user at " << sqlitePath.string();
            }
            Wt::log("info") << "UserStore ready at " << sqlitePath.string();
        } catch (const std::exception& ex) {
            Wt::log("error") << "UserStore initialization failed: " << ex.what();
        }
    }

    void loadAssets() {
        useStyleSheet("css/retro.css");
        auto script = root()->addWidget(std::make_unique<Wt::WText>(
            "<script src=\"js/retro.js\" defer></script>"));
        script->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    }

    void buildInterface() {
        root()->setId("publication-root");

        buildUserBar();
        buildMasthead();
        buildTicker();
        buildLayout();
        buildFooterNote();
    }

    void buildUserBar() {
        userBar_ = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        userBar_->setStyleClass("user-bar");
        renderUserBar();
    }

    void renderUserBar() {
        if (!userBar_) {
            return;
        }
        userBar_->clear();
        usernameField_ = nullptr;
        passwordField_ = nullptr;
        feedback_ = nullptr;

        if (!currentUser_.empty()) {
            auto status = userBar_->addWidget(std::make_unique<Wt::WText>(
                "Signed in as <strong>" + currentUser_ + "</strong>"));
            status->setTextFormat(Wt::TextFormat::UnsafeXHTML);

            auto logout = userBar_->addWidget(std::make_unique<Wt::WPushButton>("Sign out"));
            logout->setStyleClass("user-btn");
            logout->clicked().connect(this, &PublicationApp::handleLogout);
            return;
        }

        auto label = userBar_->addWidget(std::make_unique<Wt::WText>("Reader login"));
        label->setStyleClass("user-label");

        usernameField_ = userBar_->addWidget(std::make_unique<Wt::WLineEdit>());
        usernameField_->setPlaceholderText("Username");
        usernameField_->setStyleClass("user-input");

        auto passwordEdit = userBar_->addWidget(std::make_unique<Wt::WPasswordEdit>());
        passwordEdit->setPlaceholderText("Password");
        passwordEdit->setStyleClass("user-input");
        passwordField_ = passwordEdit;

        auto login = userBar_->addWidget(std::make_unique<Wt::WPushButton>("Sign in"));
        login->setStyleClass("user-btn");
        login->clicked().connect(this, &PublicationApp::handleLogin);

        feedback_ = userBar_->addWidget(std::make_unique<Wt::WText>(""));
        feedback_->setStyleClass("user-feedback");

        if (!developerMode_) {
            feedback_->setText("Production mode – ask ops to provision accounts.");
        } else {
            feedback_->setText("Use user / 123 (developer mode only).");
        }
    }

    void handleLogin() {
        if (!usernameField_ || !passwordField_) {
            return;
        }
        const std::string username = usernameField_->text().trim().toUTF8();
        const std::string password = passwordField_->text().toUTF8();

        if (username.empty() || password.empty()) {
            updateFeedback("Enter username and password.");
            return;
        }
        if (!userStore_) {
            updateFeedback("Account system offline.");
            return;
        }
        if (!userStore_->validateCredentials(username, password, developerMode_)) {
            updateFeedback("Invalid credentials or account not available.");
            passwordField_->setText("");
            return;
        }

        currentUser_ = username;
        renderUserBar();
    }

    void handleLogout() {
        currentUser_.clear();
        renderUserBar();
    }

    void updateFeedback(const std::string& message) {
        if (feedback_) {
            feedback_->setText(message);
            feedback_->setStyleClass("user-feedback user-feedback-visible");
        }
    }

    void buildMasthead() {
        auto masthead = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        masthead->setStyleClass("masthead");

        const std::string markup =
            "<p class=\"dateline\">Edition • ref_year 2012 chronology</p>"
            "<h1 class=\"brand\">Publication Ledger</h1>"
            "<p class=\"tagline\">Neutral archive, explicit extensions, evidence first.</p>";

        auto text = masthead->addWidget(std::make_unique<Wt::WText>(markup));
        text->setTextFormat(Wt::TextFormat::UnsafeXHTML);

        auto nav = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        nav->setStyleClass("section-nav");

        auto navText = nav->addWidget(std::make_unique<Wt::WText>(
            "<span>World</span><span>Universe</span><span>Multiverse</span><span>Nonfree</span><span>Opinion</span>"));
        navText->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    }

    void buildTicker() {
        static const std::vector<std::string> items = {
            "Evidence logs updated for river-delta contamination study",
            "Hypothesis 41A marked contested after auditor filing",
            "Relational offsets revised for civic budget hearings",
            "Extension filings open for public review this week"
        };

        auto ticker = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        ticker->setStyleClass("ticker");

        auto label = ticker->addWidget(std::make_unique<Wt::WText>("<span class=\"ticker-label\">Ticker</span>"));
        label->setTextFormat(Wt::TextFormat::UnsafeXHTML);

        auto track = ticker->addWidget(std::make_unique<Wt::WContainerWidget>());
        track->setStyleClass("ticker-track");

        for (const auto& item : items) {
            auto entry = track->addWidget(std::make_unique<Wt::WText>(
                "<span class=\"ticker-item\">" + item + "</span>"));
            entry->setTextFormat(Wt::TextFormat::UnsafeXHTML);
        }
    }

    void buildLayout() {
        const ArticleBrief lead {
            "Universe",
            "Council Weighs New Evidence Network for River Stewardship",
            "Ref_year −3 samples raise fresh disputes over upstream filtration claims.",
            "Dateline ≈ ref_year +11"
        };

        const std::vector<ArticleBrief> columnOne = {
            {"World", "Morning Transit Holds Steady Across Metro Lines",
             "Commuters report intervals matching ref_year averages despite operator absences.",
             "ref_year +13"},
            {"Universe", "Court Invites Briefings on Hypothesis 27F",
             "Judges request dissenting chronologies before ruling on archival disclosures.",
             "ref_year +12"},
            {"Multiverse", "Citizen Scientists Track Aurora Fluctuations",
             "Speculative readings tie solar arcs to municipal outages; evidence logs flagged provisional.",
             "ref_year +8"}
        };

        const std::vector<ArticleBrief> columnTwo = {
            {"Nonfree", "Leak Suggests Extension Pact on Port Access",
             "Confidential memo cites embargoed cargo metrics under seal; auditors notified.",
             "ref_year +10"},
            {"Opinion", "Why Explicit Extensions Matter",
             "Editorial desk argues bias tags allow evidence scrutiny without silencing dissent.",
             "ref_year +13"},
            {"Universe", "Hypothesis Cadastre Receives Public API",
             "OpsSuite opens timeline endpoints so readers can follow revision trails live.",
             "ref_year +9"}
        };

        auto layout = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        layout->setStyleClass("layout");

        auto columnLead = layout->addWidget(std::make_unique<Wt::WContainerWidget>());
        columnLead->setStyleClass("column column-front");
        buildLeadStory(columnLead, lead);
        buildStoryList(columnLead, "Front Page", columnOne);

        auto columnIndex = layout->addWidget(std::make_unique<Wt::WContainerWidget>());
        columnIndex->setStyleClass("column column-index");
        buildStoryList(columnIndex, "Ledger Highlights", columnTwo);
    }

    void buildLeadStory(Wt::WContainerWidget* container, const ArticleBrief& lead) {
        auto box = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        box->setStyleClass("lead-story");

        const std::string markup =
            "<div class=\"timestamp\">" + lead.section + " • " + lead.timestamp + "</div>"
            "<h1>" + lead.headline + "</h1>"
            "<p class=\"dek\">" + lead.dek + "</p>";

        auto text = box->addWidget(std::make_unique<Wt::WText>(markup));
        text->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    }

    void buildStoryList(Wt::WContainerWidget* container, const std::string& heading,
                        const std::vector<ArticleBrief>& stories) {
        const std::string headerMarkup = "<h2>" + heading + "</h2>";
        auto header = container->addWidget(std::make_unique<Wt::WText>(headerMarkup));
        header->setTextFormat(Wt::TextFormat::UnsafeXHTML);

        auto list = container->addWidget(std::make_unique<Wt::WText>(renderListMarkup(stories)));
        list->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    }

    std::string renderListMarkup(const std::vector<ArticleBrief>& stories) const {
        std::string html = "<ul class=\"story-index\">";
        for (const auto& story : stories) {
            html += "<li>";
            html += "<div class=\"timestamp\">" + story.section + " • " + story.timestamp + "</div>";
            html += "<div class=\"headline\">" + story.headline + "</div>";
            html += "<div class=\"dek\">" + story.dek + "</div>";
            html += "</li>";
        }
        html += "</ul>";
        return html;
    }

    void buildFooterNote() {
        auto footer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        footer->setStyleClass("footnote");

        auto text = footer->addWidget(std::make_unique<Wt::WText>(
            "Evidence networks updated on the hour. Extensions remain visible so readers track bias."));
        text->setTextFormat(Wt::TextFormat::Plain);
    }
};

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env) {
    return std::make_unique<PublicationApp>(env);
}

int main(int argc, char** argv) {
    return Wt::WRun(argc, argv, &createApplication);
}
