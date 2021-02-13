The **qt-json** project is a simple collection of functions for parsing and serializing [JSON][js] data to and from [QVariant][var] 
hierarchies.

**NOTE:** Qt5 introduced [a native JSON object class][qt5]. If you are targeting Qt5, you should use that instead.

### HOW TO USE ###
#### Parsing JSON ####

The parser is really easy to use. Let's say we have the following 
QString of JSON data:

```json
{
  "encoding" : "UTF-8",
  "plug-ins" : [
    "python",
    "c++",
    "ruby"
  ],
  "indent" : {
    "length" : 3,
    "use_space" : true
  }
}
```

We would first call the parse-function:

```cpp
#include "json.h"

bool ok;
// json is a QString containing the JSON data
QtJson::JsonObject result = QtJson::parse(json, ok).toMap();

if(!ok) {
  qFatal("An error occurred during parsing");
```

Assuming the parser completed without errors, we can then
go through the hierarchy:

```cpp
qDebug() << "encoding:" << result["encoding"].toString();
qDebug() << "plugins:";

foreach(QVariant plugin, result["plug-ins"].toList()) {
    qDebug() << "  -" << plugin.toString();
}

QtJson::JsonObject nested = result["indent"].toMap();
qDebug() << "length:" << nested["length"].toInt();
qDebug() << "use_space:" << nested["use_space"].toBool();
```

The previous code would print out the following:

    encoding: "UTF-8"
    plugins:
      - "python"
      - "c++"
      - "ruby"
    length: 3
    use_space: true


#### Serializing JSON ####
To write JSON data from Qt object is as simple as creating and assigning data to a [QVariantMap/JsonObject][varmap]:

```cpp
QtJson::JsonObject contributor;
contributor["name"] = "Luis Gustavo";
contributor["age"] = 22;

QByteArray data = QtJson::serialize(contributor);
```

The byte array 'data' contains valid JSON data:

```json
{
  "name": "Luis Gustavo",
  "age": 22
}
```

#### Serializing JSON pretty-print ####
By default, the serialization will create a _minified_ version, like following:

```json
{"name":"Luis Gustavo","age":22}
```

If you are debugging or logging, you may prefer to enable **pretty-print** mode globally, before serialize:

```cpp
QtJson::setPrettySerialize(true);

QByteArray data = QtJson::serialize(contributor);
// ...
QByteArray data = QtJson::serialize(other_contributor);
```

Obviously, you can disable it with:
```cpp
QtJson::setPrettySerialize(false);
```

---

After creating the QVariantMap, you can create a [QVariantList/JsonArray][varlist] and append the QVariantMaps. 

```cpp    
QtJson::JsonObject friend1, friend2, friend3;
friend1["id"] = 1;
friend1["name"] = "Mackenzie Hamphrey";

friend2["id"] = 2;
friend2["name"] = "Melanie Molligan";

friend3["id"] = 3;
friend3["name"] = "Sydney Calhoun";

QtJson::JsonArray friends;
friends.append(friend1);
friends.append(friend2);
friends.append(friend3);

QtJson::JsonObject obj;
obj["friends"] = friends;
```

This way you create a nested structure:

```json
{
    "friends": [
        {
            "id": 1,
            "name": "MackenzieHamphrey"
        },
        {
            "id": 2,
            "name": "MelanieMolligan"
        },
        {
            "id": 3,
            "name": "SydneyCalhoun"
        }
    ]
}
```

If you continue this process recursively, you nest more levels into the JSON structure.


#### Using Builders ####

For simplicity you can use **builders**, if you prefer.

For example, create a `JsonObject`:

```cpp
QtJson::JsonObject json = QtJson::objectBuilder()
    ->set("field_1", 10)
    ->set("field_2", "A string")
    ->set("field_3", true)
    ->set("field_4", QtJson::objectBuilder()
        ->set("sub_field_1", 10.4)
        ->set("sub_field_n", "Another string")
    )
    ->create();
```

Or create a `JsonArray`:

```cpp
QtJson::JsonArray json = QtJson::arrayBuilder()
    ->add(5)
    ->add(90.2)
    ->add(true)
    ->add("anything else")
    ->create();
```

Take a look at this example that rewrite the previous one:

```cpp
QtJson::JsonObject obj = QtJson::objectBuilder()
    ->set("friends", QtJson::arrayBuilder()
        ->add(QtJson::objectBuilder()
            ->set("id", 1)
            ->set("name", "Mackenzie Hamphrey")
        )
        ->add(QtJson::objectBuilder()
            ->set("id", 2)
            ->set("name", "Melanie Molligan")
        )
        ->add(QtJson::objectBuilder()
            ->set("id", 3)
            ->set("name", "Sydney Calhoun")
        )
    )
    ->create();
```


### 3. CONTRIBUTING ###

Send in a pull request and bug the maintainer until it gets merged and published. 
Make sure to add yourself to AUTHORS.


[js]: http://www.json.org/ "JSON Standard specification"
[var]: http://qt-project.org/doc/qt-4.8/qvariant.html "QVariant class reference"
[qt5]: http://qt-project.org/doc/qt-5.0/qtcore/qjsonobject.html "Qt5 QJsonObject class reference"
[varmap]: http://qt-project.org/doc/qt-4.8/qvariant.html#QVariantMap-typedef "Qt4 QVariantMap class reference"
[varlist]: http://qt-project.org/doc/qt-4.8/qvariant.html#QVariantList-typedef "Qt4 QVariantList class reference"
