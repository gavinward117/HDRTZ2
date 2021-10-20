import logo from './logo.svg';
import './App.css';

function App() {
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
                <button type="button" class="btn btn-color col-10">Up</button>
              </div>
              <div class="col-3">
                <button type="button" class="btn btn-color col-10">Down</button>
              </div>
              <div class="col-3">
                <button type="button" class="btn btn-color col-10">Left</button>
              </div>
              <div class="col-3">
                <button type="button" class="btn btn-color col-10">Right</button>
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
                <input type="range" class="form-range" min="0" max="100"></input>
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
                  <input class="switch-input" type="checkbox" />
                  <span class="switch-label" data-on="On" data-off="Off"></span>
                  <span class="switch-handle"></span>
                </label>
              </div>
              <div class="col-3">
                Crosshair:
              </div>
              <div class="col-3">
                <label class="switch switch-flat switch-format">
                  <input class="switch-input" type="checkbox" />
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

export default App;
