export function hasLocalStorage() {
  let item = false;
  try {
    localStorage.setItem('fnord', 23);
    item = localStorage.getItem('fnord');
    localStorage.removeItem('fnord');
  }
  catch (e) {}

  return item === 23;
}

export default hasLocalStorage;
