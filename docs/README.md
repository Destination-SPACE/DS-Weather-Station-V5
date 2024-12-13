# Destination Weather Station v5.1 Documentation
![]()
## Directory
[//]: # (Directory Links)
<div>
<h3>
<details>
  <summary><a href="#-getting-started">🚀 Getting Started</a></summary>
  <ul>
    <li><a href="#-whats-included">📝 What's Included</a></li>
    <details>
        <summary><a href=#-getting-started>🔧 Soldering</a></summary>
        <ul>
        <li><a href="#step-1">Step 1</a></li>
        <li><a href="#step-2">Step 2</a></li>
        </ul>
    </details>
    <li><a href="#final-assembly">Final Assembly</a></li>
  </ul>
</details>
</h3>

<h3>
<details>
  <summary><a href="uploading-code.md">💻 Uploading Code</a></summary>
  <ul>
    <li><a href="uploading-code.md#💾-downloading-arduino-ide">💾 Downloading Arduino IDE</a></li>
    <details>
        <summary><a href="uploading-code.md#💻-configuring-arduino-ide">💻 Configuring Arduino IDE</a></summary>
        <ul>
        <li><a href="uploading-code.md#📥-installing-board-manager">📥 Installing Board Manager</a></li>
        <li><a href="uploading-code.md#📥-installing-code-libraries">📥 Installing Code Libraries</a></li>
        </ul>
    </details>
    <li><a href="uploading-code.md#👾-example-programs">👾 Example Programs</a></li>
  </ul>
</details>
</h3>

<h3>
<details>
  <summary><a href="collecting-data.md">🗃️ Collecting Data</a></summary>
  <ul>
    <li><a href="">Domain 1</a></li>
    <details>
        <summary><a href=>Sub-Domain List 1</a></summary>
        <ul>
        <li><a href="">Sub-Domain 1</a></li>
        <li><a href="">Sub-Domain 2</a></li>
        <li><a href="">Sub-Domain 3</a></li>
        <li><a href="">Sub-Domain 4</a></li>
        <li><a href="">Sub-Domain 5</a></li>
        </ul>
    </details>
    <li><a href="">Domain 2</a></li>
  </ul>
</details>
</h3>

<h3>
<details>
  <summary><a href="collecting-data.md">🧪 Experiments</a></summary>
  <ul>
    <li><a href="">Domain 1</a></li>
    <details>
        <summary><a href=>Sub-Domain List 1</a></summary>
        <ul>
        <li><a href="">Sub-Domain 1</a></li>
        <li><a href="">Sub-Domain 2</a></li>
        <li><a href="">Sub-Domain 3</a></li>
        <li><a href="">Sub-Domain 4</a></li>
        <li><a href="">Sub-Domain 5</a></li>
        </ul>
    </details>
    <li><a href="">Domain 2</a></li>
  </ul>
</details>
</h3>
</div>

## 🚀 Getting Started

### 📝 What's Included
- 1x Weather FeatherWing
- 1x Adafruit ESP32-S3 Reverse TFT Feather
- 1x 50mAh LiPo batter
- 1x USB A to USB C cable
- 1x 8GB microSD card
- 1x microSD to USB A adapter

### 🔧 Soldering

#### Step 1:
- Find the 1x16 and 1x12 socket headers and the Adafruit ESP32-S3 Reverse TFT Feather board
- Insert the two headers onto the Feather board so that when it is laying flat on the screen, the header sockets are pointing up
- Carefully flip the board upside-down, without letting the headers fall out
- Place the tip of the soldering iron at the corner between the 1st pin on the 1x16 row and the gold pad, allowing it to heat up the board for 2-3 seconds
- Feed solder onto the pad with your other hand. The solder should melt and flow around the pad and pin
- Remove the soldering iron after 2-3 more seconds
- Repeat with the first pin on the 1x12 row
- Flip the board over and check to see if the headers are parallel to each other
- If they are, Repeat with the 16th pin on the 1x16 row and the 12th pin on the 1x12 row
- Repeat on the remaining pins for each row

#### Step 2:
- Following the same steps above, solder the short side of the 1x16 and 1x12 pin headers to the Weather FeatherWing

### Final Assembly
- Stack the Weather FeatherWing ontop of the Feather board
- Insert the microSD card into the weather station