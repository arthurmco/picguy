#include "PhotoGroupSerializer.hpp"

PhotoFormats* PhotoGroupSerializer::pfmt;

bool WriteGroupData(PhotoGroup* gp, FILE* f, int ident) {
  for (int i = 0; i < ident; i++) fputc('\t', f);
  fprintf(f, "%s { \n", gp->GetName());

  /* write first photos, then groups */
  gp->ResetPhotoIterator();
  Photo* p;
  while (p = gp->GetNextPhoto()) {
      for (int i = 0; i <= ident; i++) fputc('\t', f);
      fprintf(f, "%s\n", p->GetName());
  }

  gp->ResetGroupIterator();
  PhotoGroup* g;
  while (g = gp->GetNextGroup()) {
      if (!WriteGroupData(g, f, ++ident))
        return false;
  }

  for (int i = 0; i < ident; i++) fputc('\t', f);
  fputs(" }\n", f);
  return true;
}


bool PhotoGroupSerializer::Save(PhotoGroup* gp, const char* filename) {
  FILE* fGroup = fopen(filename, "w");

  if (!fGroup) return false;

  bool ret = WriteGroupData(gp, fGroup, 0);

  fclose(fGroup);
  return ret;
}

int lastid = 0;
bool ReadGroupData(FILE* f, PhotoGroup* gp, PhotoFormats* pfmt) {
  PhotoGroup* p;

  char cline[512];
  char cname[384];
  while (!feof(f)) {
    memset((void*)cline, 0, 512);
    memset((void*)cname, 0, 384);
    cline[0] = 0;

    /* get line */
    fgets(cline, 512, f);
    char* line;

    int off = 0;
    for (off = 0; off < 512; off++) {
        if (!isspace(cline[off]))
          break;
    }

    if (off >= 510)
        continue;

    line = &cline[off];

    /* Retrieve the name */
    int idxName = 0, idxLine = 0;
    bool group = false, file = false, groupend = false;

    for (idxLine = 0; idxLine < 384; idxLine++) {

        if (line[idxLine] == 0 && idxLine == 0)
          break;  //No content.

        if (line[idxLine] == '}' ) {
            groupend = true;
            break;
        }

      /* Check if we have a group.
         Group definitions are always <groupname>[space][bracket] */
        if (line[idxLine] == ' ' &&
            line[idxLine+1] == '{') {
            group = true;
            cname[idxName] = 0;
            break;
        }


        if (line[idxLine] == '\n' ||
            line[idxLine] == 0) {
            cname[idxName] = 0;
            file = true;
            break;
        }

        cname[idxName] = line[idxLine];
        idxName++;

    }

    if (group) {
        /* return to line start, create a group and apply the Recursion(R) */
        //fseek(f, ptr, SEEK_SET);
        printf("Group: %s\n", cname);
        p = new PhotoGroup{cname, lastid++};
        if (!ReadGroupData(f, p, pfmt))
          return false;
        gp->AddPhotoGroup(p);

    }

    if (file) {
        /* Get the file */
      //  printf("Photo: %s\n", cname);
        std::string pname = std::string{cname};
        int extindex = pname.find_last_of('.');
        std::string extension;

        if (extindex != std::string::npos)
          extension = pname.substr(extindex);
        else
          extension = "";

        Photo* ph = pfmt->GetFormat(extension.c_str());

        if (!ph)
          continue; //File not supported. (what a surprise)

        printf("Photo: %s\n", pname.c_str());
        ph->SetName(pname.c_str());
        gp->AddPhoto(ph);
    }

    if (groupend) {
      puts("Group end\n");
      return true;
    }



  }

  /* broken file, no ending brackets */
  return true;
}


void PhotoGroupSerializer::SetFormatData(PhotoFormats* fmt) {pfmt = fmt;}

/* Open the group at filename 'file' and save it on 'gp' */
bool PhotoGroupSerializer::Open(const char* filename, PhotoGroup* gp) {
  FILE* fGroup = fopen(filename, "r");
  if (!fGroup) return false;

  lastid = gp->GetID()+1;

  /* Check if the file starts with a group.
     Retries until find a group (or until the file ends)*/
  while (!feof(fGroup)) {
    char name[100];
    fgets(name, 100, fGroup);
    int namelen = strlen(name);

    if (name[namelen-3] == '{') {
        break;
    }

  }
  /* File didn't start with a group */
  if (feof(fGroup)) return false;

  bool ret = ReadGroupData(fGroup, gp, PhotoGroupSerializer::pfmt);

  fclose(fGroup);
  return ret;

}
