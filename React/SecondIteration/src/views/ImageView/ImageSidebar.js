import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/image';

import {imageViewType, settingsViewType, layoutType} from 'utils/propTypes';

import RGBAInput from 'components/inputs/RGBAInput';
import NumberInput from 'components/inputs/NumberInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';
import ColorList from 'components/inputs/ColorList';

import SidebarText from './Sidebar/Text';

import classes from './ImageSidebar.scss';

const mapStateToProps = (state) => {
  const {imageView, settingsView, layout} = state;
  return {
    imageView: imageView.toJS(),
    settingsView: settingsView.toJS(),
    layout: layout.toJS(),
  };
};

export class ImageSidebar extends Component {
  static propTypes = {
    setColorValue: PropTypes.func.isRequired, // action
    setColumns: PropTypes.func.isRequired, // action

    imageView: imageViewType,
    settingsView: settingsViewType,

    layout: layoutType,
  };

  constructor(props) {
    super(props);

    this.state = {
      visible: false,
    };
  }

  onClickShowSidebar(e) {
    this.setState({visible: !this.state.visible});
  }

  render() {
    const {
      setColorValue, setColumns, // actions
      imageView, settingsView, layout, // state objects
    } = this.props;

    const {pixels, color, rows, visibleColumns, totalColumns} = imageView;
    const {host, protocol} = settingsView;

    const {sidebar, currentColor} = layout;
    const {width} = sidebar;

    const {visible} = this.state;

    let style = {
      container: {
        width,
      },
      rgba: {},
      colorTitle: {},
      button: {
        display: layout.width > 500 ? 'none' : 'inherit',
        backgroundColor: currentColor[500],
        borderColor: currentColor[100],
      },
    };

    if (layout.width < 500) {
      style.container.position = 'absolute';
      style.container.left = -width;

      style.content = {
        left: visible ? width : 0,
        top: 25,
      };

      if (layout.width < 350) {
        style.colorTitle = {
          display: 'none',
        };

        style.rgba = {
          position: 'fixed',
          bottom: 0,
          left: '1em',
        };
      }
    }

    return (
      <aside
        className={classes['container']}
        style={style.container}
      >
        <button
          style={style.button}
          className={classes['button']}
          onClick={e => this.onClickShowSidebar(e)}
        >
          {(visible ? '<<<' : '>>>')}
        </button>

        <div
          className={classes['content']}
          style={style.content}
        >
          <div className={classes['picker']}>

            <h3>Controls</h3>
            <ul>
              <li
                key='rgba'
                style={style.rgba}
              >
                <h5 style={style.colorTitle}>Colors:</h5>
                <RGBAInput
                  color={color}
                  setColorValue={setColorValue}
                />
                <ColorList
                  editorColor={color}
                />
              </li>

              <li key='load'>
                <h5>Load Image from disk</h5>
                <ImageInput />
              </li>

              <li key='upload'>
                <h5>Upload</h5>
                <FileUploadInput
                  pixels={pixels}
                  height={rows}
                  width={visibleColumns}
                  totalWidth={totalColumns}
                  url={[protocol, host].join('://')}
                  text='send to MS3000'
                />
              </li>
            </ul>
          </div>

          <div className={classes['text']}>
            <SidebarText />
          </div>

          <div className={classes['settings']}>
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
        </div>
      </aside>
    );
  }
}

export default connect(mapStateToProps, actions)(ImageSidebar);
