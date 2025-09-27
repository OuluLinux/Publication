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
        injectInlineStyle();
        buildInterface();
    }

private:
    void configureTheme() {
        auto theme = std::make_shared<Wt::WBootstrapTheme>();
        theme->setResponsive(true);
        setTheme(theme);
        setBodyClass("publication-app");
    }

    void injectInlineStyle() {
        static const char* kInlineCss = R"CSS(
<style>
:root {
  --news-ink: #1f1a17;
  --news-page: #f3efe3;
  --news-border: #c4bca6;
  --news-highlight: #af3d2b;
  --news-muted: #5f574f;
}
body.publication-app {
  background: var(--news-page);
  color: var(--news-ink);
  font-family: "Georgia", "Times New Roman", serif;
  margin: 0;
}
#publication-root {
  max-width: 1080px;
  margin: 0 auto;
  padding: 1.5rem 1rem 3rem;
}
.masthead {
  text-align: center;
  border-bottom: 3px double var(--news-border);
  margin-bottom: 1rem;
  padding-bottom: 0.5rem;
}
.masthead .brand {
  font-size: 3rem;
  letter-spacing: 0.08em;
  text-transform: uppercase;
  margin: 0;
}
.masthead .tagline {
  font-size: 0.9rem;
  color: var(--news-muted);
  margin: 0.2rem 0 0;
}
.dateline {
  font-size: 0.85rem;
  color: var(--news-muted);
  text-transform: uppercase;
  letter-spacing: 0.18em;
  margin-top: 0.3rem;
}
.section-nav {
  display: flex;
  justify-content: center;
  gap: 1.5rem;
  border-bottom: 1px solid var(--news-border);
  border-top: 1px solid var(--news-border);
  padding: 0.45rem 0;
  margin-bottom: 1.5rem;
  font-size: 0.95rem;
  text-transform: uppercase;
  letter-spacing: 0.12em;
}
.section-nav span {
  cursor: default;
}
.layout {
  display: grid;
  grid-template-columns: 2.1fr 1.2fr;
  gap: 1.5rem;
}
.lead-story {
  border-bottom: 1px solid var(--news-border);
  padding-bottom: 1rem;
  margin-bottom: 1rem;
}
.lead-story h1 {
  font-size: 2rem;
  margin: 0;
}
.lead-story .dek {
  font-size: 1rem;
  margin: 0.6rem 0 0.8rem;
  color: var(--news-muted);
}
.timestamp {
  font-size: 0.8rem;
  color: var(--news-muted);
  text-transform: uppercase;
}
.column h2 {
  font-size: 1.2rem;
  text-transform: uppercase;
  border-bottom: 1px solid var(--news-border);
  margin: 0 0 0.5rem;
  padding-bottom: 0.4rem;
}
.story-index {
  list-style: none;
  padding: 0;
  margin: 0;
}
.story-index li {
  margin-bottom: 0.8rem;
  border-bottom: 1px dotted var(--news-border);
  padding-bottom: 0.7rem;
}
.story-index .headline {
  font-weight: 600;
  font-size: 1.05rem;
}
.story-index .dek {
  font-size: 0.9rem;
  color: var(--news-muted);
  margin-top: 0.3rem;
}
.ticker {
  background: rgba(0,0,0,0.05);
  border-top: 1px solid var(--news-border);
  border-bottom: 1px solid var(--news-border);
  padding: 0.6rem 0.8rem;
  margin-bottom: 1.5rem;
  font-size: 0.9rem;
}
.ticker strong {
  color: var(--news-highlight);
  text-transform: uppercase;
  margin-right: 0.8rem;
}
.footnote {
  text-align: center;
  font-size: 0.78rem;
  color: var(--news-muted);
  margin-top: 2rem;
  border-top: 1px solid var(--news-border);
  padding-top: 1rem;
}
@media (max-width: 900px) {
  .layout {
    grid-template-columns: 1fr;
  }
}
@media (max-width: 600px) {
  #publication-root {
    padding: 1rem 0.7rem 2.5rem;
  }
  .masthead .brand {
    font-size: 2.2rem;
  }
}
</style>
)CSS";

        auto style = root()->addWidget(std::make_unique<Wt::WText>(kInlineCss));
        style->setTextFormat(Wt::TextFormat::UnsafeXHTML);
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

        nav->addWidget(std::make_unique<Wt::WText>(
            "<span>World</span><span>Universe</span><span>Multiverse</span><span>Nonfree</span><span>Opinion</span>"))->
            setTextFormat(Wt::TextFormat::UnsafeXHTML);
    }

    void buildTicker() {
        auto ticker = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        ticker->setStyleClass("ticker");

        const std::string line =
            "<strong>Ticker</strong>Evidence logs updated for river-delta contamination study ·"
            " Hypothesis 41A marked contested · Relational offsets revised for civic budget hearings.";

        auto text = ticker->addWidget(std::make_unique<Wt::WText>(line));
        text->setTextFormat(Wt::TextFormat::UnsafeXHTML);
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
        buildLeadStory(columnLead, lead);
        buildStoryList(columnLead, "Front Page", columnOne);

        auto columnIndex = layout->addWidget(std::make_unique<Wt::WContainerWidget>());
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
