#include <windows.h>
#include <wbemidl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int set_brightness(int brightness_level){
    if (brightness_level < 0 || brightness_level > 100){
        return E_INVALIDARG;
    }
    HRESULT hr;
    IWbemLocator *pLocator = NULL;
    IWbemServices *pServices = NULL;
    IWbemClassObject *pClassObject = NULL;
    IWbemClassObject *pMethodObject = NULL;
    IWbemClassObject *pInParams = NULL;
    IEnumWbemClassObject *pEnumClassObject = NULL;
    IWbemClassObject *pQueredObjects = NULL;
    IWbemClassObject *pOutParams = NULL;
    VARIANT varTimeout;
    VARIANT varBrightness;
    VARIANT varObjectPath;
    BSTR namespace = NULL;
    BSTR class_name = NULL;
    BSTR query_language = NULL;
    BSTR query = NULL;
    BSTR bstr_method_name = NULL;

    LPCWSTR lpcwstr_method_name = NULL;
    LPCWSTR attr_name = NULL;
    ULONG reterned_count = 0;

    BOOL com_initialized = FALSE;

    VariantInit(&varTimeout);
    VariantInit(&varBrightness);
    VariantInit(&varObjectPath);

    hr = CoInitializeEx(
        NULL,
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
    );

    if (FAILED(hr)){
        goto cleanup;
    }
    if (SUCCEEDED(hr)){
        com_initialized = TRUE;
    }

    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hr) && hr != RPC_E_TOO_LATE) {
        goto cleanup;
    }

    hr = CoCreateInstance(
        &CLSID_WbemLocator,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IWbemLocator,
        (LPVOID *)&pLocator
    );

    if (FAILED(hr)){
        goto cleanup;
    }

    namespace = SysAllocString(L"ROOT\\WMI");

    if (namespace == NULL) {
        hr = E_OUTOFMEMORY;
        goto cleanup;
    }

    hr = pLocator->lpVtbl->ConnectServer(
        pLocator,
        namespace,
        NULL,
        NULL,
        NULL,
        0,
        NULL,
        NULL,
        &pServices
    );

    if (FAILED(hr)){
        goto cleanup;
    }

    hr = CoSetProxyBlanket(
        (IUnknown *)pServices,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(hr)){
        goto cleanup;
    }

    class_name = SysAllocString(L"WmiMonitorBrightnessMethods");
    
    hr = pServices->lpVtbl->GetObject(
        pServices,
        class_name,
        0,
        NULL,
        &pClassObject,
        NULL
    );

    if (FAILED(hr)){
        goto cleanup;
    }

    lpcwstr_method_name = L"WmiSetBrightness";
    hr = pClassObject->lpVtbl->GetMethod(
        pClassObject,
        lpcwstr_method_name,
        0,
        &pMethodObject,
        NULL
    );
    
    if (FAILED(hr)){
        goto cleanup;
    }

    hr = pMethodObject->lpVtbl->SpawnInstance(
        pMethodObject,
        0,
        &pInParams
    );

    if (FAILED(hr)){
        goto cleanup;
    }


    varTimeout.vt = VT_I4;
    varTimeout.lVal = 1;

    hr = pInParams->lpVtbl->Put(
        pInParams,
        L"Timeout",
        0,
        &varTimeout,
        0
    );
    if (FAILED(hr)){
        goto cleanup;
    }

    varBrightness.vt = VT_UI1;
    varBrightness.bVal = (BYTE)brightness_level;

    hr = pInParams->lpVtbl->Put(
        pInParams,
        L"Brightness",
        0,
        &varBrightness,
        0
    );
    if (FAILED(hr)){
        goto cleanup;
    }

    query_language = SysAllocString(L"WQL");
    query = SysAllocString(L"SELECT * FROM WmiMonitorBrightnessMethods");
    hr = pServices->lpVtbl->ExecQuery(
        pServices,
        query_language,
        query,
        WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
        NULL,
        &pEnumClassObject
    );
    if (FAILED(hr)){
        goto cleanup;
    }

    hr = pEnumClassObject->lpVtbl->Next(
        pEnumClassObject,
        WBEM_INFINITE,
        1,
        &pQueredObjects,
        &reterned_count
    );
    
    if ((hr != WBEM_S_NO_ERROR) || (reterned_count != 1)){
        goto cleanup;
    }

    attr_name = L"__PATH";
    hr = pQueredObjects->lpVtbl->Get(
        pQueredObjects,
        attr_name,
        0,
        &varObjectPath,
        NULL,
        NULL
    );
    
    if (FAILED(hr)){
        goto cleanup;
    }

    bstr_method_name = SysAllocString(L"WmiSetBrightness");
    hr = pServices->lpVtbl->ExecMethod(
        pServices,
        varObjectPath.bstrVal,
        bstr_method_name,
        0,
        NULL,
        pInParams,
        &pOutParams,
        NULL
    );

    if (FAILED(hr)){
        goto cleanup;
    }


    cleanup:

    if (namespace != NULL)
        SysFreeString(namespace);

    if (class_name != NULL)
        SysFreeString(class_name);
    
    if (query_language != NULL)
        SysFreeString(query_language);
    
    if (query != NULL)
        SysFreeString(query);

    if (bstr_method_name != NULL)
        SysFreeString(bstr_method_name);

    if (pServices != NULL)
        pServices->lpVtbl->Release(pServices);

    if (pLocator != NULL)
        pLocator->lpVtbl->Release(pLocator);

    if (pClassObject != NULL)
        pClassObject->lpVtbl->Release(pClassObject);
    
    if (pMethodObject != NULL)
        pMethodObject->lpVtbl->Release(pMethodObject);
    
    if (pInParams != NULL)
        pInParams->lpVtbl->Release(pInParams);
    
    if (pEnumClassObject != NULL)
        pEnumClassObject->lpVtbl->Release(pEnumClassObject);
    
    if (pQueredObjects != NULL)
        pQueredObjects->lpVtbl->Release(pQueredObjects);
    
    if (pOutParams != NULL)
        pOutParams->lpVtbl->Release(pOutParams);
    
    VariantClear(&varTimeout);
    VariantClear(&varBrightness);
    VariantClear(&varObjectPath);
    
    if (com_initialized){
        CoUninitialize();
    }

    return hr;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: brightness.exe <0-100>\n");
        return 1;
    }

    char *endptr;
    int brightness_level;

    errno = 0;
    brightness_level = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0' || errno == ERANGE) {
        printf("Invalid argument format, expected format: <0-100> [int]\n");
        return 1;
    }
    if (brightness_level < 0 || brightness_level > 100){
        printf("Invalid argument value, expected value: <0-100> [int]\n");
        return 1;
    }

    HRESULT hr;
    hr = set_brightness(brightness_level);
    if (FAILED(hr)){
        printf("Result: 0x%08lx\n", hr);
    }
    else{
        printf("Brightness successfully changed\n");
    }
    return 0;
}
