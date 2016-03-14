# glTF-parser

### what is this ?
.gltfファイルを読み込んで構造体にデータを格納するだけのソースコードです。描画等は行いません

parse .gltf file and stores the data in a structure. only that.

### requied
-std=c++11

### usage
#### construction
```C++
GLTF::glTF variable_name(filename);
```

#### データ構造
公式のspecificationドキュメントにおいて、"dictionary object of strings"と表記されているものには
`map<string, string>`を使っています。

各クラスに1個のみ格納されるオブジェクト(animation.channels.target, techniques.states.functions, camera.perspective, camera.orthographic, asset.profile)には、
`unique_ptr<foo_>`を使っています。

各クラスに配列で列挙されて格納されるオブジェクト("an array of bar"等と表記されているもの)は
`vector<bar_>`を使っています。

各クラスに連想配列で格納されるオブジェクトは
`map<string, unique_ptr<baz_> >`を使っています。

material.valuesは`unique_ptr<GLTF::object_>`型、technique.parameter.valueは`unique_ptr<GLTF::array_>`型です。（後述）

`GLTF::object_`の`data`メンバ(`map<string, void*>`型)にはデータへのポインタが格納されています。格納されうるデータの種類は以下の通りです;
* `int`
* `double`
* `bool`
* `string`
* `object_`
* `array_`
* `void *`(NULLのみ)

note: 数値リテラルに`'.'`が含まれる場合、double型を格納します。それ以外の場合はint型を格納します。


また、申し訳程度にdatatypeメンバ（`map<string, string>`型）に型名を記録しています;

* `int`         -> `"integer"`
* `double`      -> `"number"`
* `bool`        -> `"boolean"`
* `std::string` -> `"string"`
* `object_`     -> `"object"`
* `array_`      -> `"array"`
* `NULL`        -> `"void *"`

`GLTF::array_`型は`data`および`datatype`がvectorで格納されている点を除いて同じものです。

##### 配列
number型で要素数が16個に固定されている配列には`mat4`型を使っています。OpenGLの勉強をし始めた頃に作ったやつです（汗

詳しくは`linear_algebra.hpp`を参照してください。

node.rotation, scale, translationはそれぞれ`vec4`, `vec3`, `vec3`を使っています。
double型の要素を.x .y .z .w(wはvec4のみ)で参照できるようにしただけの単純な構造体です。

technique.states.functions内のメンバは各々double, int, bool型の固定配列です。

上記で挙げたもの以外の配列にはvectorを使っています。
