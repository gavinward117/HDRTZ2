import React, { Component } from 'react';
import './App.css';
import axios from 'axios';

const api = axios.create({
  baseURL: 'http://localhost:3000'
})

class App extends React.Component {

  state = {
    up: 0,
    down: 0,
    left: 0,
    right: 0,
    zoom: 0,
    mask: 0,
    crosshair: 0
  }

  constructor(props) {
    super(props);

    this.state = {
      up: 0,
      down: 0,
      left: 0,
      right: 0,
      zoom: 0,
      mask: 0,
      crosshair: 0
    }
  }

  componentDidUpdate(prevState) {
    if(this.state !== prevState){
      this.submit();
    }
  }

  moveUp = e => {
    e.preventDefault();
    console.log(this.state);
    if (this.state.up === 1) {
      this.setState({ up: 0 });
    }
    if (this.state.up === 0) {
      this.setState({ up: 1 });
    }
  }

  moveDown = e => {
    e.preventDefault();

    if (this.state.down === 1) {
      this.setState({ down: 0 });
    }
    if (this.state.down === 0) {
      this.setState({ down: 1 });
    }
  }

  moveLeft = e => {
    e.preventDefault();

    if (this.state.left === 1) {
      this.setState({ left: 0 });
    }
    if (this.state.left === 0) {
      this.setState({ left: 1 });
    }
  }

  moveRight = e => {
    e.preventDefault();

    if (this.state.right === 1) {
      this.setState({ right: 0 });
      console.log(this.state);
    }
    if (this.state.right === 0) {
      this.setState({ right: 1 });
      console.log(this.state);
    }
  }

  adjustZoom = e => {
    e.preventDefault();

    var zoomVal = document.getElementById("zoom-slider").value;
    this.setState({ zoom: zoomVal });
  }

  maskToggle = e => {
    e.preventDefault();

    var maskVal = document.getElementById("mask-toggle").value;
    this.setState({ mask: maskVal });
  }

  crosshairToggle = e => {
    e.preventDefault();

    var crosshairVal = document.getElementById("crosshair-toggle").value;
    this.setState({ crosshair: crosshairVal });
  }

  submit() {
    console.log(this.state);
    api
      .post('/', this.state)
      .then(() => console.log('Posted to api'))
      .catch(err => {
        console.error(err);
      });
  }


  render() {
    return (
      <div className="App">
        <header class="App-header">
          <div class="col-12">
            HDRTZ Configuration Menu
          </div>
        </header>


        <body class="App-body">

          <div class="row">
            <div class="col-3">
            </div>
            <div class="col-6">

              <div class="row">
                <div class="col-12">
                  Move Camera:
                </div>
              </div>
              <div class="row">
                <div class="col-3">
                  <button type="button" class="btn btn-color col-10" onClick={this.moveUp}>Up</button>
                </div>
                <div class="col-3">
                  <button type="button" class="btn btn-color col-10" onClick={this.moveDown}>Down</button>
                </div>
                <div class="col-3">
                  <button type="button" class="btn btn-color col-10" onClick={this.moveLeft}>Left</button>
                </div>
                <div class="col-3">
                  <button type="button" class="btn btn-color col-10" onClick={this.moveRight}>Right</button>
                </div>
              </div>
              <div class="row">
                &nbsp;
              </div>
              <div class="row">
                <div class="col-12">
                  Zoom:
                </div>
              </div>

              <div class="row">
                <div class="col-1">1x</div>
                <div class="col-10">
                  <input id="zoom-slider" type="range" class="form-range" onChange={this.adjustZoom} min="0" max="100"></input>
                </div>
                <div class="col-1">4x</div>
              </div>
              <div class="row">
                &nbsp;
              </div>
              <div class="row">
                <div class="col-3">
                  Mask:
                </div>
                <div class="col-3">
                  <label class="switch switch-flat switch-format">
                    <input id="mask-toggle" class="switch-input" type="checkbox" onChange={this.maskToggle}/>
                    <span class="switch-label" data-on="On" data-off="Off"></span>
                    <span class="switch-handle"></span>
                  </label>
                </div>
                <div class="col-3">
                  Crosshair:
                </div>
                <div class="col-3">
                  <label class="switch switch-flat switch-format">
                    <input id="crosshair-toggle" class="switch-input" type="checkbox" onChange={this.crosshairToggle}/>
                    <span class="switch-label" data-on="On" data-off="Off"></span>
                    <span class="switch-handle"></span>
                  </label>
                </div>
                <div class="row">
                  &nbsp;
                </div>
                <div class="row">
                  <div class="col-3">
                  </div>
                  <div class="col-6">
                    <button type="button" class="btn btn-color col-10">Calibrate Crank When Idle</button>
                  </div>
                  <div class="col-3">
                  </div>
                </div>
              </div>
            </div>
            <div class="col-3">
            </div>
          </div>
        </body>
      </div>
    );
  }
}

export default App;
