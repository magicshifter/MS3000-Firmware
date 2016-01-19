import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/image';

import {layoutType} from 'utils/propTypes';

import NumberInput from 'components/inputs/NumberInput';

import classes from './Settings.scss';

const mapStateToProps = (state) => {
  const {imageView, layout} = state;

  return {
    totalColumns: imageView.get('totalColumns'),
    visibleColumns: imageView.get('visibleColumns'),
    layout: layout.toJS(),
  };
};

export class Settings extends Component {
  static propTypes = {
    setColumns: PropTypes.func.isRequired, // action

    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,

    layout: layoutType,
  };

  render() {
    const {
      setColumns, // action
      totalColumns,
      visibleColumns,
    } = this.props;

    return (
      <div
        className={classes['container']}
      >
        <h3>Settings</h3>

        <ul>
          <li>
            <NumberInput
              label='Columns:'
              name='columns'
              max={totalColumns}
              val={visibleColumns}
              action={setColumns}
            />
          </li>
        </ul>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Settings);
