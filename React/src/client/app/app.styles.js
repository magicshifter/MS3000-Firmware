
export default {
  header: {
    container: {
      display: 'inline-block',
      width: '100%',
      padding: '1em 2em 0',
      height: '130px',
      backgroundColor: '#039BE5',
    },
    background: {
      backgroundImage: 'url(/assets/img/header-background.png)',
      backgroundSize: 'cover',
      height: '130px',
      top: 0,
      left: 0,
      position: 'absolute',
      width: '100%',
    },
    statusBar: {
      height: '1em',
      fontSize: '.8em',
      backgroundColor: '#D4E157',
      padding: '.5em .5em .5em 1em',
      borderBottom: '5px solid #B0BEC5',
    },
    h1: {
      float: 'left',
      margin: '.5em',
      fontSize: '2em',
      fontWeight: 600,
      color: '#fff',
    },
    link: {
      float: 'left',
      position: 'relative',
    },
    span: {
      width: '100%',
      textAlign: 'center',
      display: 'block',
      marginTop: '-.5vw',
    },
    nav: {
      float: 'right',
      position: 'relative',
    },
    li: {
      fontSize: '1em',
      float: 'left',
      display: 'inline-block',
      margin: '0 0 0 .5em',
    },
  },

  footer: {
    fontSize: '.75em',
    fontStyle: 'italic',
    position: 'absolute',
    bottom: 0,
  },
};
