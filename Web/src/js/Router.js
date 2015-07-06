import Store from './Store';
import riot from 'riot';

export class Router extends Store {
  constructor(hq) {
    super();
    hq.enlist(this);
    this._broadcast = hq.emit.bind(hq);

    this.current = false;
    this.on('Router:redirect', (path) => this.redirect(path));
    riot.route(this.observe.bind(this));
  }

  page(node, path = false, opts = false) {
    if (!opts && typeof path === 'object') {
      opts = path;
      path = false;
    }

    this.enlist(node);
    node.on('Router:change', (path) => this.view(node, path));
    node.path = `${path || node.root.nodeName.toLowerCase()}`;
  }

  observe(...args) {
    let path = args.join('/');
    if (!path.length) { return; }

    this.current = path;
    this.emit('Router:change', path);
  }

  view(node, path = '') {
    node.show = (path.replace('#', '') === node.path.replace('#', ''));
    node.update();
  }

  redirect(path = false) {
    if (path && path.length) {
      this.current = path;
      riot.route(path);
    }

    this.emit('Router:change', path);
  }
}

export default Router;
