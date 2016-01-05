// need to import react here
import React, {Component, PropTypes} from 'react';
import Radium from 'radium';

import PixelEditor from './PixelEditor/PixelEditor';
import styles from '../css/base';

@Radium
export default class Root extends Component {

  static propTypes = {
    text: PropTypes.string,
  }

  static defaultProps = {
    text: 'Example Component default text',
  }

  constructor(props) {
    super(props);

    this.state = {
      text: props.text,
      clicked: 0,
    };
  }

  onButtonClick() {
    const clicked = this.state.clicked + 1;
    this.setState({
      text: `You Clicked the Button ${this.state.clicked} times!`,
      clicked,
    });
  }

  render() {
    const {text} = this.state;

    return (
      <div
        className='pixelEditor'
        style={styles.body}
      >
        <PixelEditor
          rows={16}
          columns={16}
        />
        <div>
          <p>{text}</p>
          <button
            onClick={this.onButtonClick.bind(this)}
          >
            Click Me
          </button>
        </div>
      </div>
    );
  }
}
