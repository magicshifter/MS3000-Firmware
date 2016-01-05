
export default {
  header: {
    wrapper: {
      display: 'inline-block',
      width: '100%',
      padding: '1em 2em 0',
      height: '150px',
      backgroundColor: '#039BE5',
      position: 'relative',
    },
    container: {
      maxWidth: '800px',
      margin: '0 auto',
    },
    background: {
      backgroundImage: 'url(/assets/img/header-background.png)',
      backgroundSize: 'cover',
      height: '140px',
      top: 0,
      left: 0,
      position: 'absolute',
      width: '100%',
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
    margin: '3em 0',
  },
};
