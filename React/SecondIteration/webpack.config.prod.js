var path = require('path');
var webpack = require('webpack');

module.exports = {
  devtool: 'source-map',
  entry: [
    './src/index'
  ],
  output: {
    path: path.join(__dirname, 'dist', 'js'),
    filename: 'bundle.js',
    publicPath: '/js/'
  },
  plugins: [
    new webpack.optimize.OccurenceOrderPlugin(),
    new webpack.DefinePlugin({
      'process.env': {
        'NODE_ENV': JSON.stringify('production')
      }
    }),
    new webpack.optimize.UglifyJsPlugin({
      compressor: {
        warnings: false
      }
    })
  ],
  module: {
     loaders: [
      {
        test: /\.(jsx|js)$/,
        loader: ['babel'],
        include: path.join(__dirname, 'src'),
        query: {
          presets: ['react', 'es2015'],
          plugins: ['transform-class-properties', 'transform-decorators-legacy',],
          cacheDirectory: true,
        },
      },
      {
        test: /\.(gif|jpg|png|woff|woff2|eot|ttf|svg)$/,
        loader: 'url-loader?limit=100000',
      },
    ],
  }
};
