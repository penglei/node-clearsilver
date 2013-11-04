{
  "targets": [
    {
      "target_name": "clearsilver",
      "sources": [
        "src/clearsilver-wrap.cc",
        "src/node-clearsilver.cc"
      ],
      "dependencies":[
        'clearsilver/src/neolib.gyp:neo',
      ],
    },
    {
        "target_name":"cs-runner",
        "type":"none",
        "dependencies":[
          "clearsilver/build/all.gyp:*"
        ]
    }
  ]
}
