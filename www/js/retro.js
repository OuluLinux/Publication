(function() {
  function duplicateTickerContent(track) {
    if (!track) return;
    var original = track.innerHTML;
    track.innerHTML = original + original;
  }

  function addTickerHover(track, container) {
    if (!track || !container) return;
    container.addEventListener('mouseenter', function() {
      track.style.animationPlayState = 'paused';
    });
    container.addEventListener('mouseleave', function() {
      track.style.animationPlayState = 'running';
    });
  }

  function subtlyTintHeadlines() {
    var headlines = document.querySelectorAll('.story-index .headline');
    headlines.forEach(function(el, index) {
      el.style.animationDelay = (index * 0.25) + 's';
      el.classList.add('headline-glow');
    });
  }

  document.addEventListener('DOMContentLoaded', function() {
    var track = document.querySelector('.ticker-track');
    var ticker = document.querySelector('.ticker');
    duplicateTickerContent(track);
    addTickerHover(track, ticker);

    subtlyTintHeadlines();
  });
})();
