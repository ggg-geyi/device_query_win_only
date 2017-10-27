{
	
	"conditions" : [
	["target_arch=='ia32'", {
	'variables': {
            'openssl_root%': 'C:/openssl'
          },
          }],
  ["target_arch=='x64'", {
	'variables': {
            'openssl_root%': 'C:/openssl-x64'
          },
          }],    
    ],
          
  "targets": [
    {
      "target_name": "DeviceQuery",
      "conditions" : [
            ['OS == "win"', {
      				 "sources": [ "src/DeviceQuery.cc","src/WMI_DeviceQuery.cpp","src/IOCTL_DeviceQuery.cpp","src/RSA_EncryptData.cpp" ],
      				 'libraries': [ '-l<(openssl_root)/lib/libeay32.lib','-l<(openssl_root)/lib/ssleay32.lib',],
      				 'include_dirs': [  '<(openssl_root)/include', ],
      			}],
        	  ['OS == "mac"', {
      				 "sources": [ "src/DeviceQueryForMac.cc"],
      			}],
      ],
     }
  ]
}