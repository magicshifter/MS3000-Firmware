import React, {Component} from 'react';

export default class ExampleComponent extends Component {
  constructor(props) {
    super(props);
    this.props = {
      text: 'Example Component',
    };
  }

  render() {
    var text = this.props.text;

    return (
      <div>{text}</div>
    );
  }
}

