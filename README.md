# Publication

A modular news publication and information system that labels political bias as extensions and categorizes stories using Debian-inspired sections (`world`, `universe`, `multiverse`, `nonfree`, and beyond). The platform emphasizes evidence networks, hypothesis graphs, transparent revisions, AI awareness of knowledge gaps, and neutral archival of conflicts without taking sides.

## Key Ideas
- **Extensions:** Each desk declares its political orientation so readers understand the lens behind coverage.
- **Debian Sections:** Content falls into `world` (self-evident experiences), `universe` (reporting anchored to the current evidence network and ready to be overturned by new proof), `multiverse` (fringe or contested narratives), and `nonfree` (restricted-access investigations).
- **Evidence Networks:** Every claim traces back to documented evidence, with change logs when consensus shifts.
- **Hypothesis Networks:** Competing chronologies and interpretations coexist as graph nodes with explicit provenance, so stories cite exactly which hypotheses they rely on.
- **Relational Chronology:** Dates are expressed relative to a reference year (currently 2012) or anchor events, accommodating multiple temporal hypotheses.
- **AI Modularity:** Assistive tooling acknowledges blind spots, keeps domain boundaries clear, and defers judgment when evidence is thin.

## Development Workflow
- **Primary IDE:** Ultimate++ TheIDE provides code navigation, assists, and build orchestration tailored to our C++ workflow.
- **Source Layout:** Application code lives under `/src`. Each package occupies `/src/<PackageName>/` and contains a matching `.upp` descriptor (e.g., `/src/EvidenceViewer/EvidenceViewer.upp`).
- **Build Targets:** Every package can produce both a standalone Wt server (development) and a FastCGI binary (production behind Apache).
- **Tooling Interop:** CMake files accompany packages to support CI and alternative editors while TheIDE remains authoritative during local development.

## Package Layout
- `CoreInfra` – configuration, logging, storage connectors, evidence/hypothesis data models.
- `WtKernel` – shared Wt bootstrap, routing, authentication, deployment toggles (FastCGI vs standalone).
- `PublicSuite` – aggregates the public-facing modules for the combined landing and timeline experience.
- `InternalSuite` – aggregates investigative tooling for staff-only workflows.
- `OpsSuite` – aggregates identity, integrations, and deployment helpers.
- `FrontPortal` – landing experience with trending threads, evidence highlights, neutral archival messaging.
- `Streamline` – microblog timeline, composer, boosts/replies, follow graph, evidence tagging.
- `MomentsDesk` – curated narratives bundling posts, evidence timelines, and hypothesis snapshots.
- `ChroniclePosts` – long-form investigations and articles with relational chronology views.
- `Commentarium` – comments and moderation queue embedded across posts and articles.
- `SignalForums` – topic channels, community discussions, and moderation tools.
- `EvidenceExplorer` – internal dashboards for evidence networks and competing chronologies.
- `MessagingDeck` – direct and team messaging for coordination around hypotheses.
- `SyncStream` – synchronized audio stream (our not-radio service) with server-controlled playlists and MP3 playback across visitors.
- `ArchiveVault` – downloadable file archive with checksums, version history, and access policies.
- `AccountsDesk` – profiles, verification tiers, political extensions, permissions, audit trails.
- `ApiGateway` – REST/WebSocket interfaces for clients and integrations.
- `StaticAssets` – stylesheet and script bundling utilities.
- `DeploymentTools` – build scripts, packaging, and environment provisioning.
- `PublicationApp` – top-level target tying everything together for standalone or FastCGI deployment.

See `AGENTS.md` for the full agency charter.
