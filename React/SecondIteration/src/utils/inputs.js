
export const onInputChange =
  (e, self) => {
    const { name, value } = e.target;
    self.setState({ [name]: value });
  };
