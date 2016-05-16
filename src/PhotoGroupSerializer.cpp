#include "PhotoGroupSerializer.hpp"

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
