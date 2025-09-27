#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEnvironment.h>
#include <Wt/WText.h>

#include <memory>
#include <string>
#include <vector>

namespace {
struct ArticleBrief {
    std::string section;
    std::string headline;
    std::string dek;
    std::string timestamp;
};
}

class PublicationApp : public Wt::WApplication {
public:
    explicit PublicationApp(const Wt::WEnvironment& env)
        : Wt::WApplication(env) {
        setTitle("Publication");
        configureTheme();
        loadAssets();
        buildInterface();
    }

private:
    void configureTheme() {
        auto theme = std::make_shared<Wt::WBootstrapTheme>();
        theme->setResponsive(true);
        setTheme(theme);
        setBodyClass("publication-app");
    }

    void loadAssets() {
        useStyleSheet("css/retro.css");
        auto script = root()->addWidget(std::make_unique<Wt::WText>(
            "<script src=\"js/retro.js\" defer></script>"));
        script->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    }

    void buildInterface() {
        root()->setId("publication-root");

        buildMasthead();
        buildTicker();
        buildLayout();
        buildFooterNote();
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
