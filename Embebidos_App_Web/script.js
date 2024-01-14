// Función para actualizar los datos
function fetchData() {
    fetch('http://192.168.100.166/data')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok ' + response.statusText);
            }
            return response.json();
        })
        .then(data => {
            updateUISoil(data.moisture_value);
            updateUIAir(data.hum);
            updateUITemp(data.temp);
            updateUILedV(data.LED_VERDE);
            updateUILedR(data.LED_ROJO);
            updateUIWaterPump(data.bomba);
        })
        .catch(error => {
            console.error('Error:', error)
        });
}

// ------------------------------------------------
// Funciones para actualizar la interfaz de usuario
// ------------------------------------------------

// Actualizar humedad del suelo
function updateUISoil(moisture_value) {
    const soilProgBar = document.getElementById('soil_humidity_bar');
    const soilHumContBar = document.getElementById('soil_hum_cont_bar');
    soilHumContBar.style.backgroundColor = getHumSoilColor(moisture_value);
    soilProgBar.style.width = `${moisture_value}%`;
    soilProgBar.style.backgroundColor = getHumSoilColor(moisture_value, false);
    document.getElementById('soil_humidity_value').textContent = `${moisture_value}%`;
}

// Actualizar humedad del aire
function updateUIAir(hum) {
    const airHumidityBar = document.getElementById('air_humidity_bar');
    const airHumContBar = document.getElementById('air_hum_cont_bar');
    airHumContBar.style.backgroundColor = getHumAirColor(hum);
    airHumidityBar.style.width = `${hum}%`;
    airHumidityBar.style.backgroundColor = getHumAirColor(hum, false);
    document.getElementById('air_humidity_value').textContent = `${hum}%`;
}

// Actualizar termómetro de temperatura
function updateUITemp(temp) {
    const tempThermometer = document.getElementById('temp_thermometer');
    tempThermometer.style.backgroundColor = getTempColor(temp);
    document.getElementById('temp_value').textContent = `${temp}°C`;
}

// Actualizar indicador LED verde
function updateUILedV(LED_VERDE) {
    const ledVerdeIndicator = document.getElementById('LED_VERDE_indicator');
    ledVerdeIndicator.style.backgroundColor = LED_VERDE ? '#18c418' : 'grey';
    document.getElementById('LED_VERDE').textContent = LED_VERDE ? 'Activado' : 'Desactivado';
}

// Actualizar indicador LED rojo
function updateUILedR(LED_ROJO) {
    const ledRojoIndicator = document.getElementById('LED_ROJO_indicator');
    ledRojoIndicator.style.backgroundColor = LED_ROJO ? 'red' : 'grey';
    document.getElementById('LED_ROJO').textContent = LED_ROJO ? 'Activado' : 'Desactivado';
}

// Actualizar la bomba de agua
function updateUIWaterPump(bomba) {
    const bombaEstado = document.getElementById('bomba_estado');
    const motorBomba = document.getElementById('motor_bomba');
    if (bomba) {
        bombaEstado.textContent = 'Activada';
        motorBomba.classList.add('active');
    } else {
        bombaEstado.textContent = 'Desactivada';
        motorBomba.classList.remove('active');
    }
}

// ------------------------------------------------
// Funciones para actualizar los colores background
// ------------------------------------------------

// Determinar el color del termómetro
function getTempColor(temp) {
    if (temp < 19) return 'lightblue';
    if (temp >= 19 && temp < 29) return 'green';
    return 'red';
}

// Determinar el color de la barra de humedad del suelo
function getHumSoilColor(moisture_value, parent=true) {
    if (parent) {
        if (moisture_value < 30) return '#bb8888';
        return 'lightskyblue';
    }else{
        if (moisture_value < 30) return 'red';
        return 'blue';
    }
}

// Determinar el color de la barra de humedad del aire
function getHumAirColor(hum, parent=true) {
    if (parent) {
        if (hum < 40) return '#bb8888';
        return 'lightskyblue';
    } else {
        if (hum < 40) return 'red';
        return 'blue';
    }
}

// Llamar a fetchData cada 2 segundos
setInterval(fetchData, 2000);
