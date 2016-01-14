import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {pixelEditorType, settingsType, layoutType} from 'utils/propTypes';

import RGBAInput from 'components/inputs/RGBAInput';
import NumberInput from 'components/inputs/NumberInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './PixelEditorSidebar.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, settings, layout} = state;
  return {
    pixelEditor: pixelEditor.toJS(),
    settings: settings.toJS(),
    layout: layout.toJS(),
  };
};

export class PixelEditorSidebar extends Component {
  static propTypes = {
    setColorValue: PropTypes.func.isRequired,
    setColumns: PropTypes.func.isRequired,

    pixelEditor: pixelEditorType,
    settings: settingsType,
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
      pixelEditor, settings, layout, // state objects
    } = this.props;

    const {pixels, color, rows, visibleColumns, totalColumns} = pixelEditor;
    const {host, protocol} = settings;

    const {sidebar} = layout;
    const {width} = sidebar;

    const {visible} = this.state;

    let style = {
      container: {
        width,
      },
      button: {
        display: layout.width > 500 ? 'none' : 'inherit',
      },
    };

    if (layout.width < 500) {
      style.container.position = 'absolute';
      style.container.right = 0;

      style.content = {
        right: !visible ? -200 : 0,
      };
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
          {visible ? 'Hide' : 'Show'} Sidebar
        </button>

        <div
          className={classes['content']}
          style={style.content}
        >
          <div className={classes['picker']}>

            <h3>Controls</h3>

            <RGBAInput
              color={color}
              setColorValue={setColorValue}
            />

            <ImageInput label='upload image' />

            <FileUploadInput
              pixels={pixels}
              height={rows}
              width={visibleColumns}
              totalWidth={totalColumns}
              url={[protocol, host].join('://')}
              text='send to MS3000'
            />
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

export default connect(mapStateToProps, actions)(PixelEditorSidebar);
