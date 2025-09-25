# Agency Charter

## Mission
We operate a news publication and information system that documents events and ideas across the political spectrum while remaining explicit about bias. Each published item identifies its political extension so readers understand the perspective and evidentiary posture behind the work. Our role is neutral archiving—we document conflicts without intervening in their outcomes.

## Extensions and Categories
- **Extension (Political Bias):** Every desk declares its political bias as an "extension" tag accompanying the content.
- **Debian-Inspired Sections:**
  - `world`: Everyday, broadly observable facts and experiences considered self-evident for the average person.
  - `universe`: Reporting grounded in the current evidence network, always open to refutation by any credible counter-evidence.
  - `multiverse`: Fringe, emerging, or contested ideas, including speculative science and unverified narratives.
  - `nonfree`: Investigations contingent on proprietary data, confidential sources, or restricted access.
  - Additional categories may be added as editorial policy evolves.

## Evidence Networks
- Treat evidence collections as configurable networks that define current understanding.
- Maintain the history of network changes so shifts in consensus remain transparent.
- Highlight dissent, missing data, and reported issues alongside corroborating evidence.

## Hypothesis Networks
- Model competing interpretations as hypothesis graphs, with nodes for each claim or timeline and edges describing support, dependency, or conflict.
- Store temporal variants for an event as distinct hypotheses tied to the relational chronology protocol.
- Record provenance: who proposed the hypothesis, when, and under which political extension or investigative desk.
- Track adoption so published pieces cite the exact hypotheses they rely on, enabling targeted retractions or updates.
- Maintain version histories so investigators can audit how hypotheses evolve alongside incoming evidence.

## Relational Chronology Protocol
- Anchor timelines to a documented reference year (`ref_year`), currently set to 2012, and keep justification for the anchor in view.
- Express dates as offsets from the reference: `event_X ≈ ref_year − 73` rather than absolute calendar years.
- When another event yields a clearer reference, specify the relation: `event_A in context of event_B`, with both tied back to the anchor.
- Cite the evidence supporting each offset and note the confidence level (likely, approximate, speculative).
- Maintain revision logs so new evidence updates offsets without rewriting prior narratives.
- Allow multiple temporal hypotheses per event so competing chronologies can coexist until evidence converges.

## Science Posture
- Science coverage respects the provisional nature of knowledge: conclusions only stand as long as supporting evidence remains unchallenged.
- Avoid presenting science as settled unless the evidence network shows sustained support and no unresolved issues.

## AI Awareness and Modularity
- AI assistants must acknowledge potential blind spots, especially regarding fresh evidence not yet in the model.
- AI outputs should remain modular: avoid crossing subject domains or evidence networks without human review.
- When uncertainty is high, prefer withholding judgments over unverified claims.

## Publishing Principles
- Trace every claim back to sources within the relevant evidence network.
- Flag content when evidence is incomplete, contradictory, or relies on disputed assumptions.
- Ensure political extensions and category tags remain visible to readers and internal reviewers.

## Accountability
- Periodically audit published material for outdated evidence, broken links, or superseded findings.
- Document corrections and retractions with the associated evidence network updates.
- Encourage readers and contributors to report new evidence that may shift category placement or bias assumptions.
