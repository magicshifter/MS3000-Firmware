
export const onInputChange =
  (e) => {
    const {name, value} = e.target;
    this.setState({[name]: value});
  };
