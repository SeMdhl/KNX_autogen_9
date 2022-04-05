#pragma once
#ifndef mdhl_Knx
#define mdhl_Knx
#include <iostream>
#include <fstream>
#include <windows.h>
#include "mdhl_Knx_Items_v9.h"
#include "mdhl_Tools.h"







void WriteXML_KNX(std::string sPath, bool(&bUsed)[1000], std::string& sGVL, std::string(&sRom)[1000], std::string(&sRomtype563)[1000], std::string(&sRomtypeKnx)[1000], std::string(&sKommentar)[1000], std::string& sAdresseFormat, int const& iMax)
{
    //Variabel deklarering
    int iZero = 0, iSpan = 0, iMaster = 1, iKnx = 0, iMasterMax = 1, iKnxOutputs = -1, iCfc_Id = 0, iCfc_Order = 0, iCfc_y = 3, iCfc_x = 2, iSize = 0, iAntall = 0, iTemp = 0, iLast1 = 0, iLast2 = 0;
    std::string sPreset = "";
    bool xComment = true, xSpace = false, xRomstyring = false;


    sPath = sPath + "AutGenImport.xml";

    //Kopierer XML filens adresse, til utklippstavle
    const char* output = sPath.c_str();
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();

    //Lager xml filen
    std::ofstream fOutput(sPath);

    if (fOutput.is_open() == false)
    {
        std::cout << "Error: cant open outputfile.\nLine: 908 mdhl.h";
        Sleep(10000);
        abort();
    }
    //Skriver begynnelse av XML filen, slik e!cockpit gjør under export.
    fOutput << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    fOutput << "<project xmlns=\"http://www.plcopen.org/xml/tc6_0200\">\n\t";
    fOutput << "<fileHeader companyName=\"WAGO\" productName=\"e!COCKPIT\" productVersion=\"e!COCKPIT\" creationDateTime=\"2021-01-18T14:20:23.2110188\" />\n\t";
    fOutput << "<contentHeader name=\"Adressering.ecp\" modificationDateTime=\"2021-01-18T11:51:27.6823754\">\n\t\t";
    fOutput << "<coordinateInfo>\n" + Tabs(3);
    fOutput << "<fbd>\n" + Tabs(4);
    fOutput << "<scaling x=\"1\" y=\"1\" />\n" + Tabs(3);
    fOutput << "</fbd>\n" + Tabs(3);
    fOutput << "<ld>\n" + Tabs(4);
    fOutput << "<scaling x=\"1\" y=\"1\" />\n" + Tabs(3);
    fOutput << "</ld>\n" + Tabs(3);
    fOutput << "<sfc>\n" + Tabs(4);
    fOutput << "<scaling x=\"1\" y=\"1\" />\n" + Tabs(3);
    fOutput << "</sfc>\n\t\t";
    fOutput << "</coordinateInfo>\n\t\t";
    fOutput << "<addData>\n" + Tabs(3);
    fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/projectinformation\" handleUnknown=\"implementation\">\n" + Tabs(4);
    fOutput << "<ProjectInformation />\n" + Tabs(3);
    fOutput << "</data>\n\t\t";
    fOutput << "</addData>\n\t";
    fOutput << "</contentHeader>\n\t";
    fOutput << "<types>\n\t\t";
    fOutput << "<dataTypes>\n\t\t";


    std::string sDatatypes[100];
    bool xUnik = false;

    //initializer array
    for (int i = 0; i < 100; i++)
    {
        sDatatypes[i] = "";
    }

    bool xLh = false, xLc = false;

    //Skriver datatyper som er brukt
    for (int i = 0; i < iMax; i++)
    {
        xLh = xLc = false;
        if (bUsed[i])
        {
            xUnik = true;
            for (int j = 0; j < 100; j++)
            {
                if (sRomtype563[i] == sDatatypes[j])
                {
                    xUnik = false;
                    break;
                }
            }
            if (xUnik)
            {
                if (fOutput.is_open() == false)
                    fOutput.open(sPath, std::ios::app);

                fOutput << "<dataType name=\"dtRomtype_" << sRomtype563[i] << "\">\n" + Tabs(3);
                fOutput << "<baseType>\n" + Tabs(4);
                fOutput << "<struct>\n" + Tabs(5);

                fOutput.close();

                iSize = sRomtype563[i].size();

                for (int j = 0; j < iSize; j++)
                {
                    xComment = true;
                    iAntall = (stoi(sRomtype563[i].substr(j, 1)));
                    if (j < sRomtype563[i].size() && iAntall >= 1)
                    {
                        switch (j)
                        {
                        case 0:
                            Knx_dt_Rb(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 1:
                            Knx_dt_Hvac(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 2:
                            Knx_dt_Rt(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 3:
                            Knx_dt_Ry(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 4:
                            Knx_dt_Lh_OP(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            xLh = true;
                            break;

                        case 5:
                            Knx_dt_Lh_CMD(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall, xLh);
                            iKnxOutputs += iAntall;
                            break;

                        case 6:
                            Knx_dt_Lc_OP(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            xLc = true;
                            break;

                        case 7:
                            Knx_dt_Lc_CMD(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall, xLc);
                            iKnxOutputs += iAntall;
                            break;

                        case 8:
                            Knx_dt_Sp(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 9:
                            Knx_dt_Sp_Fb(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 10:
                            Knx_dt_Lu_V(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 11:
                            Knx_dt_Lu_Al(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 12:
                            Knx_dt_Lu_Cmd(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        case 13:
                            Knx_dt_Opm_Out(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs+= iAntall;
                            break;

                        default:
                            break;
                        }
                    }
                }
                fOutput.open(sPath, std::ios::app);

                fOutput << "</struct>\n" + Tabs(3);
                fOutput << "</baseType>\n" + Tabs(3);
                fOutput << "<addData />\n" + Tabs(2);
                fOutput << "</dataType>\n" + Tabs(1);

                for (int j = 0; j < 100; j++)
                {
                    if (sDatatypes[j] == "")
                    {
                        sDatatypes[j] = sRomtype563[i];
                        break;
                    }
                }
            }
        }
        else
            break;
    }

    fOutput << "</dataTypes>\n\t\t";
    fOutput << "<pous>\n\t\t";


    //Lager Funksjonsblokker (kun innganger og utganger. ingen funksjon/styring
    xLh = xLc = false;
    for (int i = 0; i < iMax; i++)
    {
        xLh = false, xLc = false;
        iSize = sDatatypes[i].size();
        if (sDatatypes[i] != "")
        {
            if (iSize > 3)
            {
                fOutput << "<pou name = \"fbRomtype_" << sDatatypes[i] << "\" pouType = \"functionBlock\">\n\t";
                fOutput << "<interface>\n\t";
                fOutput << "<inputVars>\n\t";

                fOutput.close();

                for (int j = 0; j < iSize; j++)
                {
                    iAntall = (stoi(sDatatypes[i].substr(j, 1)));
                    if (iAntall > 0)
                    {
                        switch (j)
                        {
                        case 1:
                            Knx_Fb_In_Rb(sPath, iAntall);
                            break;

                        case 2:
                            Knx_Fb_In_Rt(sPath, iAntall);
                            break;

                        case 3:
                            Knx_Fb_In_Ry(sPath, iAntall);
                            break;

                        case 4:
                            if (fOutput.is_open() == false)
                                fOutput.open(sPath, std::ios::app);

                            fOutput << "<variable name = \"LH601_MAN\">\n\t";
                            fOutput << "<type>\n\t";
                            fOutput << "<BOOL/>\n\t";
                            fOutput << "</type>\n\t";
                            fOutput << "</variable>\n\t";
                            fOutput.close();
                            xLh = true;
                            break;

                        case 5:
                            if (!xLh)
                            {
                                if (fOutput.is_open() == false)
                                    fOutput.open(sPath, std::ios::app);

                                fOutput << "<variable name = \"LH601_MAN\">\n\t";
                                fOutput << "<type>\n\t";
                                fOutput << "<BOOL/>\n\t";
                                fOutput << "</type>\n\t";
                                fOutput << "</variable>\n\t";
                                fOutput.close();
                            }
                            break;

                        case 6:
                            if (fOutput.is_open() == false)
                                fOutput.open(sPath, std::ios::app);

                            fOutput << "<variable name = \"LC601_SP\">\n\t";
                            fOutput << "<type>\n\t";
                            fOutput << "<REAL/>\n\t";
                            fOutput << "</type>\n\t";
                            fOutput << "</variable>\n\t";

                            fOutput << "<variable name = \"LC601_MAN\">\n\t";
                            fOutput << "<type>\n\t";
                            fOutput << "<BOOL/>\n\t";
                            fOutput << "</type>\n\t";
                            fOutput << "</variable>\n\t";
                            fOutput.close();
                            xLc = true;
                            break;

                        case 7:
                            if (!xLc)
                            {
                                if (fOutput.is_open() == false)
                                    fOutput.open(sPath, std::ios::app);

                                fOutput << "<variable name = \"LC601_SP\">\n\t";
                                fOutput << "<type>\n\t";
                                fOutput << "<REAL/>\n\t";
                                fOutput << "</type>\n\t";
                                fOutput << "</variable>\n\t";

                                fOutput << "<variable name = \"LC601_MAN\">\n\t";
                                fOutput << "<type>\n\t";
                                fOutput << "<BOOL/>\n\t";
                                fOutput << "</type>\n\t";
                                fOutput << "</variable>\n\t";
                                fOutput.close();
                            }
                            break;

                        case 8:
                            Knx_Fb_In_Sp_In(sPath, iAntall);
                            break;

                        case 10:
                            Knx_Fb_In_Lu_V(sPath, iAntall);
                            break;

                        default:
                            break;
                        }
                    }
                }

                if (fOutput.is_open() == false)
                    fOutput.open(sPath, std::ios::app);

                fOutput << "</inputVars>\n\t";
                fOutput << "<outputVars>\n\t";
                fOutput.close();

                for (int j = 0; j < iSize; j++)
                {
                    iAntall = (stoi(sDatatypes[i].substr(j, 1)));
                    if (iAntall > 0)
                    {
                        switch (j)
                        {
                        case 2:
                            Knx_Fb_Out_Rt(sPath, iAntall);
                            break;

                        case 3:
                            if (fOutput.is_open() == false)
                                fOutput.open(sPath, std::ios::app);

                            fOutput << "<variable name = \"RY601_OP\">\n\t";
                            fOutput << "<type>\n\t";
                            fOutput << "<REAL/>\n\t";
                            fOutput << "</type>\n\t";
                            fOutput << "</variable>\n\t";
                            fOutput.close();
                            break;

                        case 4:
                            Knx_Fb_Out_Lh_Op(sPath, iAntall);
                            break;
                        case 5:;
                            Knx_Fb_Out_Lh_Cmd(sPath, iAntall);
                            break;

                        case 6:
                            Knx_Fb_Out_Lc_Op(sPath, iAntall);
                            break;

                        case 7:
                            Knx_Fb_Out_Lc_Cmd(sPath, iAntall);
                            break;

                        case 9:
                            //Knx_Fb_Out_Sp(sPath, iAntall);        Er ikke ment til å være egen SP out. Bruker bare RT601_SP_CS
                            break;

                        case 12:
                            Knx_Fb_Out_Lu_Cmd(sPath, iAntall);
                            break;

                        case 13:
                            Knx_Fb_Out_Opm_Out(sPath, iAntall);
                            break;

                        default:
                            break;
                        }
                    }
                }

                if (fOutput.is_open() == false)
                    fOutput.open(sPath, std::ios::app);

                fOutput << "</outputVars>\n\t";
                fOutput << "</interface>\n\t";
                fOutput << "<body>\n\t";
                fOutput << "<ST>\n\t";
                fOutput << "<xhtml xmlns = \"http://www.w3.org/1999/xhtml\"/>\n\t";
                fOutput << "</ST>\n\t";
                fOutput << "<addData>\n\t";
                fOutput << "<data name = \"http ://www.3s-software.com/plcopenxml/cfc\" handleUnknown = \"implementation\">\n\t";
                fOutput << "<CFC/>\n\t";
                fOutput << "</data>\n\t";
                fOutput << "</addData>\n\t";
                fOutput << "</body>\n\t";
                fOutput << "<addData/>\n\t";
                fOutput << "</pou>\n\t";
            }
        }
        else
            break;
    }

    //Finner ut hvor mange knx linjer som trengs
    for (int i = 0; i < iMax; i++)
    {
        if (bUsed[i])
        {
            iTemp = 0;
            iSize = sRomtypeKnx[i].size();
            for (int j = 0; j < iSize; j++)
            {
                iAntall = stoi((sRomtypeKnx[i].substr(j, 1)));
                if (iAntall >= 1)
                    iTemp += iAntall;
            }
            if ((iKnx + iTemp) == 255)
            {
                iMasterMax++;
                iKnx = 0;
            }
            else if ((iKnx + iTemp) > 255)
            {
                iMasterMax++;
                iKnx = 0;
                i--;
            }
            else
                iKnx += iTemp;
        }
        else
            break;
    }

    //Lager alle KNX linjer brukt
    for (int k = 0; k < iMasterMax; k++)
    {
        iMaster = k + 1;
        iKnx = 0;
        iCfc_Id = 0;
        iCfc_Order = 0;
        iCfc_y = 2;
        iCfc_x = 2;
        //Finner max knx verdi for nåværende linje
        iTemp = 0;
        for (int i = 0; i < iMax; i++)
        {
            if (bUsed[i])
            {
                iSize = sRomtypeKnx[i].size();
                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtypeKnx[i].substr(j, 1));
                    if (iAntall >= 1)
                        iTemp += iAntall;
                }
            }
            else
                break;
        }

        iSpan = iTemp / iMasterMax;
        iKnx = 1;


        if (fOutput.is_open() == false)
            fOutput.open(sPath, std::ios::app);

        fOutput << "<pou name=\"PRG_KNX_563_" << iMaster << "\" pouType=\"program\">\n" + Tabs(3);
        fOutput << "<interface>\n" + Tabs(4);
        fOutput << "<localVars>\n" + Tabs(5);



        //Local Var

        if (iKnxOutputs >= 0)
        {
            fOutput << "<variable name=\"i\">\n" + Tabs(6);
            fOutput << "<type>\n" + Tabs(7);
            fOutput << "<INT />\n" + Tabs(6);
            fOutput << "</type>\n" + Tabs(6);
            fOutput << "<initialValue>\n" + Tabs(7);
            fOutput << "<simpleValue value=\"0\" />\n" + Tabs(6);
            fOutput << "</initialValue>\n" + Tabs(6);
            fOutput << "<documentation>\n" + Tabs(7);
            fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\">Felles</xhtml>\n" + Tabs(6);
            fOutput << "</documentation>\n" + Tabs(5);
            fOutput << "</variable>\n" + Tabs(5);

            fOutput << "<variable name=\"Interval\">\n" + Tabs(6);
            fOutput << "<type>\n" + Tabs(7);
            fOutput << "<derived name=\"TON\"/>\n" + Tabs(6);
            fOutput << "</type>\n" + Tabs(5);
            fOutput << "<initialValue>\n" + Tabs(6);
                fOutput << "<structValue>\n" + Tabs(7);
                fOutput << "<value member = \"PT\">\n" + Tabs(8);
                fOutput << "<simpleValue value = \"TIME#2m0s0ms\"/>\n" + Tabs(7);
                fOutput << "</value>\n" + Tabs(6);
                fOutput << "</structValue>\n" + Tabs(6);
                fOutput << "</initialValue>\n" + Tabs(5);
            fOutput << "</variable>\n" + Tabs(5);
        }
        //Resetter teller
        iKnxOutputs = -1;

        fOutput << "<variable name=\"typDPT\">\n" + Tabs(6);
        fOutput << "<type>\n" + Tabs(7);
        fOutput << "<derived name=\"typDPT\" />\n" + Tabs(6);
        fOutput << "</type>\n" + Tabs(5);
        fOutput << "</variable>\n" + Tabs(5);

        fOutput << "<variable name=\"KNX_master_" << iMaster << "\">\n" + Tabs(6);
        fOutput << "<type>\n" + Tabs(7);
        fOutput << "<derived name=\"WagoAppKNX.FbKNX_Master\" />\n" + Tabs(6);
        fOutput << "</type>\n" + Tabs(6);
        fOutput << "<documentation>\n" + Tabs(7);
        fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\">Master</xhtml>\n" + Tabs(6);
        fOutput << "</documentation>\n" + Tabs(5);
        fOutput << "</variable>\n" + Tabs(5);

        fOutput.close();

        //Skriver lokalvariabler
        for (int i = iZero; i < 256; i++)
        {
            if (bUsed[i] && iKnx <= iSpan)
            {
                iSize = sRomtypeKnx[i].size();
                xComment = true;
                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtypeKnx[i].substr(j, 1));
                    if (j < sRomtypeKnx[i].size() && iAntall >= 1)
                    {
                        switch (j)
                        {
                        case 0:
                            Knx_var_Rb(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 1:
                            Knx_var_Hvac(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 2:
                            Knx_var_Rt(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 3:
                            Knx_var_Ry(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 4:
                            Knx_var_Lh_OP(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        case 5:
                            Knx_var_Lh_CMD(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        case 6:
                            Knx_var_Lc_OP(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        case 7:
                            Knx_var_Lc_CMD(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        case 8:
                            Knx_var_Sp(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 9:
                            Knx_var_Sp_Fb(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 10:
                            Knx_var_Lu_V(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 11:
                            Knx_var_Lu_Al(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            break;

                        case 12:
                            Knx_var_Lu_Cmd(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        case 13:
                            Knx_var_Opm_Out(sPath, iMaster, &iKnx, sRom[i], &xComment, iAntall);
                            iKnxOutputs += iAntall;
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
            else
                break;
        }

        //Åpner output fil igjen
        fOutput.open(sPath, std::ios::app);

        //Ender lokal variabler, og starter på program
        fOutput << "</localVars>\n" + Tabs(3);
        fOutput << "</interface>\n" + Tabs(3);
        fOutput << "<body>\n" + Tabs(4);
        fOutput << "<ST>\n" << Tabs(5);
        fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\" />\n" << Tabs(5);
        fOutput << "</ST>\n" << Tabs(4);
        fOutput << "<addData>\n" << Tabs(5);
        fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/cfc\" handleUnknown=\"implementation\">\n" << Tabs(4);
        fOutput << "<CFC>\n\t";

        fOutput.close();    //Lukker fil før funksjoner kjøres

        //Skriv CFC kode for KNX ouput interval
        if (iKnxOutputs >= 0)
        {
            Knx_cfc_Interval(sPath, iKnxOutputs);
            iCfc_Id = 22;
            iCfc_Order = 7;
        }
        Knx_cfc_Master(sPath, sGVL, sAdresseFormat, iMaster, &iCfc_Order, &iCfc_Id);

        //Resetter Knx variabler brukt i lokal variabel deklarering
        iKnx = 1;
        iKnxOutputs = 0;

        //Lager CFC program
        for (int i = iZero; i < 256; i++)
        {
            if (bUsed[i] && iKnx <= iSpan)
            {
                Knx_cfc_comment_a(sPath, sRom[i], &iCfc_Id, iCfc_y);

                iSize = sRomtypeKnx[i].size();

                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtypeKnx[i].substr(j, 1));
                    if (j < sRomtypeKnx[i].size() && iAntall >= 1)
                    {
                        switch (j)
                        {
                        case 0:
                            Knx_cfc_Rb(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 1:
                            Knx_cfc_Hvac(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 2:
                            Knx_cfc_Rt(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 3:
                            Knx_cfc_Ry(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 4:
                            Knx_cfc_Lh_OP(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, &iKnxOutputs, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 5:
                            Knx_cfc_Lh_CMD(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, &iKnxOutputs, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 6:
                            Knx_cfc_Lc_OP(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, &iKnxOutputs, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 7:
                            Knx_cfc_Lc_CMD(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, &iKnxOutputs, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 8:
                            Knx_cfc_Sp(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 9:
                            Knx_cfc_Sp_Fb(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 10:
                            Knx_cfc_Lu_V(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 11:
                            Knx_cfc_Lu_Al(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 12:
                            Knx_cfc_Lu_CMD(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, &iKnxOutputs, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        case 13:
                            Knx_cfc_Opm_Out(sPath, sGVL, sAdresseFormat, sRom[i], iMaster, &iKnx, &iCfc_Order, &iCfc_Id, &iCfc_y, &iCfc_x, &xSpace, &iKnxOutputs, iAntall);
                            iLast2 = iLast1;
                            iLast1 = j;
                            break;

                        default:
                            break;
                        }
                    }
                }
                if (xSpace)
                {
                    iCfc_x = 2;
                    iCfc_y += 8;
                }
                else if ((iLast1 < 2 || iLast1 == 5 || iLast1 == 7 || iLast1 < 9) && (iLast2 < 2 || iLast2 == 5 || iLast2 == 7 || iLast1 < 9))
                {
                    iCfc_x = 2;
                    iCfc_y += 26;
                }
                else
                {
                    iCfc_x = 2;
                    iCfc_y += 27;
                }
            }
            else
                break;
        }
        iZero = iSpan;

        fOutput.open(sPath, std::ios::app);

        fOutput << "</CFC>\n" + Tabs(1);
        fOutput << "</data>\n" + Tabs(1);
        fOutput << "</addData>\n" + Tabs(1);
        fOutput << "</body>\n" + Tabs(1);
        fOutput << "<addData />\n" + Tabs(1);
        fOutput << "</pou>\n" + Tabs(1);
    }

    //nytt
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Romstyring programm
    iKnx = iCfc_Id = iCfc_Order = iCfc_y = 0;
    iCfc_y = -18;


    //Temp variabler for å telle inputs og outputs. Mer inn/outputs = mer io pluss på y akse
    int iInOut = 0, iFb = 0, iHeight = 0;
    xLh = xLc = false;


    fOutput << "<pou name=\"PRG_563_Romstyring" << "\" pouType=\"program\">\n" + Tabs(3);
    fOutput << "<interface>\n" + Tabs(4);
    fOutput << "<localVars>\n" + Tabs(5);
    fOutput.close();

    for (int i = 0; i < iMax; i++)
    {
        if (bUsed[i])
        {
            iSize = sRomtype563[i].size();
            if (iSize > 3)
            {
                Knx_var_Romstyring_Fb(sPath, sRom[i], sRomtype563[i]);
            }
        }
        else
            break;
    }

    fOutput.open(sPath, std::ios::app);
    fOutput << "</localVars>\n" + Tabs(3);
    fOutput << "</interface>\n" + Tabs(3);
    fOutput << "<body>\n" + Tabs(4);
    fOutput << "<ST>\n" << Tabs(5);
    fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\" />\n" << Tabs(5);
    fOutput << "</ST>\n" << Tabs(4);
    fOutput << "<addData>\n" << Tabs(5);
    fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/cfc\" handleUnknown=\"implementation\">\n" << Tabs(4);
    fOutput << "<CFC>\n\t";
    fOutput.close();


    for (int i = 0; i < iMax; i++)
    {
        xLh = xLc = false;
        if (bUsed[i])
        {
            iSize = sRomtype563[i].size();
            if (iSize > 3)
            {
                //comment
                Knx_cfc_Fb_Comment(sPath, sRom[i], &iCfc_Id, &iCfc_y);


                //Inputs
                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtype563[i].substr(j, 1));
                    if (iAntall > 0)
                    {
                        switch (j)
                        {
                        case 1:
                            Knx_cfc_In_Hvac(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        case 2:
                            Knx_cfc_In_Rt(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        case 3:
                            Knx_cfc_In_Ry(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        case 4:
                            Knx_cfc_In_Lh(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            xLh = true;
                            break;

                        case 5:
                            if (!xLh)
                                Knx_cfc_In_Lh(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        case 6:
                            Knx_cfc_In_Lc(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            xLc = true;
                            break;

                        case 7:
                            if (!xLc)
                                Knx_cfc_In_Lc(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        case 9:
                            Knx_cfc_In_Sp(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        case 10:
                            Knx_cfc_In_Lu_V(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Id, &iCfc_y, iAntall, &iInOut);
                            break;

                        default:
                            //std::cout << "Error: Cant find In var of type" << j << "\n";
                            break;
                        }
                    }
                }

                xLh = xLc = false;

                //Lager funksjonsblokk
                fOutput.open(sPath, std::ios::app);
                fOutput << "<block localId=\"" << (0 + iCfc_Id) << "\" executionOrderId=\"" << (0 + iCfc_Order) << "\" typeName=\"fbRomtype_" << sRomtype563[i] << "\" instanceName=\"Rom_" << sRom[i] << "\">\n\t";
                fOutput << "<position x=\"" << (26) << "\" y=\"" << (28 + iCfc_y) << "\" />\n\t";
                fOutput << "<inputVariables>\n\t";
                iCfc_Id++;
                iCfc_Order++;
                fOutput.close();


                //FB inputs
                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtype563[i].substr(j, 1));
                    if (iAntall > 0)
                    {
                        switch (j)
                        {
                        case 1:
                            Knx_cfc_In_Fb_Hvac(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;

                        case 2:
                            Knx_cfc_In_Fb_Rt(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;

                        case 3:
                            Knx_cfc_In_Fb_Ry(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;

                        case 4:

                            Knx_cfc_In_Fb_Lh(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            xLh = true;
                            break;

                        case 5:
                            if (!xLh)
                                Knx_cfc_In_Fb_Lh(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;

                        case 6:
                            Knx_cfc_In_Fb_Lc(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            xLc = true;
                            break;

                        case 7:
                            if (!xLc)
                                Knx_cfc_In_Fb_Lc(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;

                        case 9:
                            Knx_cfc_In_Fb_Sp(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;

                        case 10:
                            Knx_cfc_In_Fb_Lu_V(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, iInOut, &iFb);
                            break;
                        default:
                            //std::cout << "Error: Cant find input of type" << j << "\n";
                            break;
                        }
                    }
                }


                //Ender fb inputs, starter outputs
                fOutput.open(sPath, std::ios::app);
                fOutput << "</inputVariables>\n\t";
                fOutput << "<inOutVariables/>\n\t";
                fOutput << "<outputVariables>\n\t";
                fOutput.close();

                iHeight = iFb;

                //FB outputs
                iInOut = iFb = 0;
                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtype563[i].substr(j, 1));
                    if (iAntall > 0)
                    {
                        switch (j)
                        {
                        case 2:
                            Knx_cfc_Fb_Out_Rt(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 3:
                            Knx_cfc_Fb_Out_Ry_Op(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 4:
                            Knx_cfc_Fb_Out_Lh_Op(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 5:
                            Knx_cfc_Fb_Out_Lh_Cmd(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 6:
                            Knx_cfc_Fb_Out_Lc_Op(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 7:
                            Knx_cfc_Fb_Out_Lc_Cmd(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 12:
                            Knx_cfc_Fb_Out_Lu_Cmd(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        case 13:
                            Knx_cfc_Fb_Out_Opm_Out(sPath, &iCfc_y, iAntall, &iFb);
                            break;

                        default:
                            //std::cout << "Error: Cant find output of type" << j << "\n";
                            break;
                        }
                    }
                }

                if (iFb > iHeight)
                    iHeight = iFb;

                //Ender funksjonsblokk
                fOutput.open(sPath, std::ios::app);
                fOutput << "</outputVariables>\n\t";
                fOutput << "<addData>\n\t";
                fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/cfccalltype\" handleUnknown=\"implementation\">\n\t";
                fOutput << "<CallType xmlns=\"\">functionblock</CallType>\n\t";
                fOutput << "</data>\n\t";
                fOutput << "</addData>\n\t";
                fOutput << "</block>\n\t";
                fOutput.close();



                //Outputs
                iInOut = 0;
                for (int j = 0; j < iSize; j++)
                {
                    iAntall = stoi(sRomtype563[i].substr(j, 1));
                    if (iAntall > 0)
                    {
                        switch (j)
                        {
                        case 2:
                            Knx_cfc_Out_Rt(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 3:
                            Knx_cfc_Out_Ry_Op(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 4:
                            Knx_cfc_Out_Lh_Op(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 5:
                            Knx_cfc_Out_Lh_Cmd(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 6:
                            Knx_cfc_Out_Lc_Op(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 7:
                            Knx_cfc_Out_Lc_Cmd(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 12:
                            Knx_cfc_Out_Lu_Cmd(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        case 13:
                            Knx_cfc_Out_Opm_Out(sPath, sGVL, sAdresseFormat, sRom[i], &iCfc_Order, &iCfc_Id, &iCfc_y, iAntall, &iInOut, iFb);
                            break;

                        default:
                            //std::cout << "Error: Cant find out var of type" << j << "\n";
                            break;
                        }
                    }
                }
                iCfc_y += 14 + iHeight;;
                iInOut = iFb = 0;
            }
        }
        else
            break;
    }
    fOutput.open(sPath, std::ios::app);

    fOutput << "</CFC>\n" + Tabs(1);
    fOutput << "</data>\n" + Tabs(1);
    fOutput << "</addData>\n" + Tabs(1);
    fOutput << "</body>\n" + Tabs(1);
    fOutput << "<addData />\n" + Tabs(1);
    fOutput << "</pou>\n" + Tabs(1);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fOutput << "<pou name=\"PRG_360_HVAC\" pouType=\"program\">\n" + Tabs(3);
    fOutput << "<interface>\n" + Tabs(4);
    fOutput << "<localVars>\n" + Tabs(3);
    fOutput << "</localVars>\n" + Tabs(3);
    fOutput << "</interface>\n" + Tabs(3);
    fOutput << "<body>\n" + Tabs(2);
    fOutput << "<ST>" << std::endl << Tabs(1);
    fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\">";

    fOutput << sGVL << "." << sAdresseFormat << "_RB001_HVAC:=\n(\n";
    bool xFirst = true;
    for (int i = 0; i < iMax && bUsed[i]; i++)
    {
        iTemp = stoi(sRomtype563[i].substr(1, 1));
        for (int j = 1; j <= iTemp; j++)
        {
            if (xFirst)
            {
                fOutput << "\t" << sGVL << "." << sAdresseFormat << "_" << sRom[i] << ".RB60" << j << "_HVAC";
                xFirst = false;
            }
            else
                fOutput << " OR\n\t" << sGVL << "." << sAdresseFormat << "_" << sRom[i] << ".RB60" << j << "_HVAC";
        }
    }
    fOutput << "\n);";

    fOutput << "</xhtml>\n";
    fOutput << "</ST>\n" + Tabs(3);
    fOutput << "</body>\n" + Tabs(3);
    fOutput << "<addData/>\n" + Tabs(2);
    fOutput << "</pou>\n" + Tabs(2);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //GVL
    fOutput << "</pous>\n" + Tabs(1);
    fOutput << "</types>\n" + Tabs(1);
    fOutput << "<instances>\n" + Tabs(1);
    fOutput << "<configurations />\n" + Tabs(1);
    fOutput << "</instances>\n" + Tabs(1);
    fOutput << "<addData>\n" + Tabs(1);
    fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/globalvars\" handleUnknown=\"implementation\">\n" + Tabs(2);
    fOutput << "<globalVars name=\"" << sGVL <</*"\" retain=\"true\" persistent=\"true\*/"\">\n" + Tabs(3);

    fOutput << "<variable name=\"" << sAdresseFormat << "_RB001_HVAC" << "\">\n" + Tabs(4);
    fOutput << "<type>\n" + Tabs(5);
    fOutput << "<BOOL />\n" + Tabs(4);
    fOutput << "</type>\n" + Tabs(4);
    fOutput << "<documentation>\n" + Tabs(4);
    fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\">HVAC signal for PLS</xhtml>\n" + Tabs(3);
    fOutput << "</documentation>\n" + Tabs(3);
    fOutput << "</variable>\n" + Tabs(3);

    for (int i = 0; i < iMasterMax; i++)
    {
        iMaster = i + 1;
        fOutput << "<variable name=\"" << sAdresseFormat << "_KNX_Card_" << iMaster << "_Status" << "\">\n" + Tabs(4);
        fOutput << "<type>\n" + Tabs(5);
        fOutput << "<BOOL />\n" + Tabs(4);
        fOutput << "</type>\n" + Tabs(4);
        fOutput << "<documentation>\n" + Tabs(4);
        fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\">KNX kort " << iMaster << " status</xhtml>\n" + Tabs(3);
        fOutput << "</documentation>\n" + Tabs(3);
        fOutput << "</variable>\n" + Tabs(3);
    }

    for (int i = 0; i < iMax; i++)
    {
        if (bUsed[i])
        {

            fOutput << "<variable name=\"" << sAdresseFormat << "_" << sRom[i] << "\">\n" + Tabs(4);
            fOutput << "<type>\n" + Tabs(5);
            fOutput << "<derived name=\"dtRomtype_" << sRomtype563[i] << "\" />\n" + Tabs(4);
            fOutput << "</type>\n" + Tabs(4);
            fOutput << "<documentation>\n" + Tabs(4);
            fOutput << "<xhtml xmlns=\"http://www.w3.org/1999/xhtml\">" << sKommentar[i] << "</xhtml>\n" + Tabs(3);
            fOutput << "</documentation>\n" + Tabs(3);
            fOutput << "</variable>\n" + Tabs(3);
        }
        else
            break;
    }
    fOutput << "<addData>\n" << Tabs(4);

    fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/attributes\" handleUnknown=\"implementation\">\n" + Tabs(5);
    fOutput << "<Attributes>\n" + Tabs(6);
    fOutput << "<Attribute Name=\"qualified_only\" Value=\"\" />\n" + Tabs(5);
    fOutput << "</Attributes>\n" + Tabs(4);
    fOutput << "</data>\n" + Tabs(4);

    fOutput << "<data name=\"http://www.3s-software.com/plcopenxml/buildproperties\" handleUnknown=\"implementation\">\n" + Tabs(5);
    fOutput << "<BuildProperties>\n" + Tabs(6);
    fOutput << "<LinkAlways>true</LinkAlways>\n" + Tabs(5);
    fOutput << "</BuildProperties>\n" + Tabs(4);
    fOutput << "</data>\n" + Tabs(3);
    fOutput << "</addData>\n" + Tabs(2);
    fOutput << "</globalVars>\n" + Tabs(1);
    fOutput << "</data>\n";
    fOutput << "</addData>\n";
    fOutput << "</project>" << std::endl;

    fOutput.close();
}
#endif