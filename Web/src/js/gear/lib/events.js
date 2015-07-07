/*
 * Function stopEvent
 * stops propagation and bubbling of events.
 *
 * @param {Event} evt - event to preventDefault and stopPropagation
 */
export function stopEvent(evt = {}) {
  if (evt.stopPropagation) {
    evt.stopPropagation();
  } else {
    evt.cancelBubble = true;
  }

  if (evt.preventDefault) {
    evt.preventDefault();
  } else {
    evt.returnValue = false;
  }

  return false;
}
