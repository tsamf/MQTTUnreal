# MQTTUnreal  
Created a smart home monitoring system using the MQTT Standard in Unreal. Publishers (smart objects) were created using esp32 controllers and a MQTT Arduino library.  

<img src="/Documentation/MQTT_Data_Flow.png" width="750">

## MQTT Broker  
Installed a Mosquitto MQTT Broker on Raspberry Pi 3 Model B.   
The broker is responsible for routing traffic between publishers and subscribers.  

<img src="/Documentation/PI_Broker.jpg" width="750">

## MQTT Client Publisher  
Using an ESP32 and the Arduino IDE, created a publisher. The publisher connects to Wi-Fi, the MQTT Broker, and then subscribes to a topic for each defined output pin.  
When a button is pressed a message is published to the corresponding topic.  

<img src="/Documentation/Publisher_Client.jpg" width="750">
<img src="/Documentation/Publisher_Client_On.jpg" width="750">

## MQTT Client Subscriber  
The client subscriber is implemented using Unreal.    
Leveraging a MQTT plugin, created blueprints for client connection and smart objects that subscribe to topics and react based on received status.      
<img src="/Documentation/Blueprints.png" width="750">

Once a map is created a client and any number of smart objects can be placed in the scene.     
The client will handle the MQTT connection and find all of the smart objects in the scene by their class type and subscribe to their corresponding topics.    
When the ESP Client Publisher sends an update, the unreal client will receive the message and route it to the correct smart object based on the topic.     
<img src="/Documentation/Map_Outliner.png" width="750">

The light smart object is simulated with a cylinder that is hidden when the LED is off and shows when the LED is on.     
<img src="/Documentation/Running_All_Lights.jpg" width="750">     
Two LEDs off.     
<img src="/Documentation/Running_Far_Lights.jpg" width="750">     
