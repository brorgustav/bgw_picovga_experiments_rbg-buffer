

// #include "hardware/adc.h"
// // ADC Input API

// // ADCInput(pin0 [, pin1, pin2, pin3])

// // Creates an ADC input object which will record the pins specified in the code. Only pins A0 … A3 can be used, and they must be specified in increasing order (i.e. ADCInput(A0, A1); is valid, but ADCInput(A1, A0) is not.

// // bool setBuffers(size_t buffers, size_t bufferWords)

// // Set the number of DMA buffers and their size in 32-bit words. Call before ADCInput::begin().

// // When running at high sample rates, it is recommended to increase the bufferWords to 32 or higher (i.e. adcinput.setBuffers(4, 32); ).

// // bool setPins(pin_size_t pin [, pin1, pin2, pin3])

// // Adjusts the pin to record. Only legal before ADCInput::begin().

// // bool setFrequency(long sampleRate)

// // Sets the ADC sampling frequency, but does not start recording (however if the device was already running, it will continue to run at the new frequency). Note that every pin requested will be sampled at this frequency, one after the other. That is, if you have code like this:

// // ADCInput adc(A0, A1);
// // adc.setFrequency(1000);
// // A0 will be sampled at 0ms, 1ms, 2ms, etc. and A1 will be sampled at 0.5ms 1.5ms, 2.5ms, etc. Each input is sampled at the proper frequency but offset in time since there is only one active ADC at a time.

// // bool begin()/begin(long sampleRate)

// // Start the ADC input up with the given sample rate, or with the value set using the prior setFrequency call.

// // void end()

// // Stops the ADC Input device.

// // int read()

// // Reads a single sample of recorded ADC data, as a 16-bit value. When multiple pins are recorded the first read will be pin 0, the second will be pin 1, etc. Applications need to keep track of which pin is being returned (normally by always reading out all pins at once). Will not return until data is available.

// // int available()

// // Returns the number of samples that can be read without potentially blocking.

// // void onReceive(void (*fn)(void))

// // Sets a callback to be called when a ADC input DMA buffer is fully filled. Will be in an interrupt context so the specified function must operate quickly and not use blocking calls like delay().

// // Develop and launch modern apps with MongoDB Atlas, a resilient data platform.
// // Ads by
// // Analog input pins to buffer


// #define ADC_RED_PIN A0
// #define ADC_GREEN_PIN A1
// #define ADC_BLUE_PIN A2
// // Creates an ADC input object which will record the pins specified in the code. 
// // Only pins A0 … A3 can be used, and they must be specified in increasing order 
// // (i.e. ADCInput(A0, A1); is valid, but ADCInput(A1, A0) is not.
// ADCInput adc_red(A0);
// ADCInput adc_green(A0);
// ADCInput adc_blue(A0);
// // Set the number of DMA buffers and their size in 32-bit words. Call before ADCInput::begin().
// // When running at high sample rates, it is recommended to increase the bufferWords to 32 or higher (i.e. adcinput.setBuffers(4, 32); ).
// // bool setBuffers(size_t buffers, size_t bufferWords)
// ADCInput setBuffers(3, pixels]); 
// ADCInput begin(8000);

// void read_adc()
// {

// }

// void insert_adc()
// {

// }